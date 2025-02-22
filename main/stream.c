#include <esp_http_client.h>
#include <esp_log.h>
#include <stddef.h>

#include "stream.h"
#include "cJSON.h"

static const char *TAG = "cb_http";

static bool NEW_GAME;
static bool NEW_POSITION;
static char FEN_BUF[128];

void cb_handle_data_event(char* data, int len) {
  cJSON* json = cJSON_ParseWithLength(data, len);
  if (json == NULL) {
    const char* err_ptr = cJSON_GetErrorPtr();
    ESP_LOGE(TAG, "Parse of event data failed with error \"%s\", data \"%.*s\"", err_ptr, len, data);
    return;
  }
  const cJSON* type = cJSON_GetObjectItem(json, "t");
  if (!cJSON_IsString(type) || type->valuestring == NULL) {
    ESP_LOGE(TAG, "Event data contains faulty type");
    goto END;
  }
  ESP_LOGI(TAG, "Event type %s", type->valuestring);
  const cJSON* j_data = cJSON_GetObjectItem(json, "d");
  const cJSON* fen = cJSON_GetObjectItem(j_data, "fen");
  if (!cJSON_IsString(fen) || type->valuestring == NULL) {
    ESP_LOGE(TAG, "Event data contains faulty fen");
    goto END;
  }
  strcpy(FEN_BUF, fen->valuestring);
  NEW_POSITION = true;
  if (strcmp(type->valuestring, "featured") == 0)
    NEW_GAME = true;
END:
  cJSON_Delete(json);
}

esp_err_t cb_http_event_handler(esp_http_client_event_t *evt) {
  switch (evt->event_id) {
  case HTTP_EVENT_ON_DATA:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
    cb_handle_data_event((char*)evt->data, evt->data_len);
    break;
  case HTTP_EVENT_ERROR:
    ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
    break;
  case HTTP_EVENT_ON_CONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
    break;
  case HTTP_EVENT_HEADERS_SENT:
    ESP_LOGI(TAG, "HTTP_EVENT_HEADERS_SENT");
    break;
  case HTTP_EVENT_ON_HEADER:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
    break;
  case HTTP_EVENT_ON_FINISH:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
    break;
  case HTTP_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
    break;
  case HTTP_EVENT_REDIRECT:
    ESP_LOGI(TAG, "HTTP_EVENT_REDIRECT");
    break;
  }
  return ESP_OK;
}

void cb_http_stream(void*) {
    esp_http_client_handle_t client;
    esp_http_client_config_t config = {
        .url               = "https://lichess.org/api/tv/bullet/feed",
        .method            = HTTP_METHOD_GET,
        .auth_type         = HTTP_AUTH_TYPE_NONE,
        .event_handler     = cb_http_event_handler,
        .keep_alive_enable = true
    };
    NEW_GAME     = false;
    NEW_POSITION = false;
    while (true) {
      client = esp_http_client_init(&config);
      esp_http_client_perform(client);
      esp_http_client_cleanup(client);
    }
}
