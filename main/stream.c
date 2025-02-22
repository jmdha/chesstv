#include <esp_http_client.h>
#include <esp_log.h>
#include <stddef.h>

#include "stream.h"

static const char *TAG = "cb_http";

static bool NEW_GAME;
static bool NEW_POSITION;
static char FEN_BUF[128];

esp_err_t cb_http_event_handler(esp_http_client_event_t *evt) {
  switch (evt->event_id) {
  case HTTP_EVENT_ON_DATA:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
    printf("%.*s\n", evt->data_len, (char*)evt->data);
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
