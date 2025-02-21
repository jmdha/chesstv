#include <esp_event.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include <nvs_flash.h>

#define SSID CONFIG_ESP_WIFI_SSID
#define PASS CONFIG_ESP_WIFI_PASSWORD

static EventGroupHandle_t s_wifi_event_group;

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    printf("disconnected from wifi\n");
    printf("attempting to reconnect\n");
    esp_wifi_connect();
  }
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
    printf("connected to wifi\n");
  }
}

void init_nvs() {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
}

void init_wifi() {
  s_wifi_event_group = xEventGroupCreate();
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t config_init = WIFI_INIT_CONFIG_DEFAULT();
  printf("run esp_wifi_init\n");
  ESP_ERROR_CHECK(esp_wifi_init(&config_init));
  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

  wifi_config_t config_wifi = {
      .sta = {.ssid = SSID,
              .password = PASS,
              .threshold.authmode = WIFI_AUTH_WPA2_PSK}};
  printf("run esp_wifi_set_mode\n");
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  printf("run esp_wifi_set_config\n");
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &config_wifi));
  printf("run esp_wifi_start\n");
  ESP_ERROR_CHECK(esp_wifi_start());
  printf("run esp_wifi_connect\n");
  ESP_ERROR_CHECK(esp_wifi_connect());
  printf("wifi_init finished\n");
}

void app_main(void) {
  init_nvs();
  init_wifi();
  while (1) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
