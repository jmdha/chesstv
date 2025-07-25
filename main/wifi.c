#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include <freertos/event_groups.h>

#include "wifi.h"

#define SSID CONFIG_ESP_WIFI_SSID
#define PASS CONFIG_ESP_WIFI_PASSWORD

const char *TAG = "wifi";

bool CONNECTED;
EventGroupHandle_t s_wifi_event_group;

void event_handler(
	void*,
	esp_event_base_t base,
	int32_t type,
	void*
) {
	if (base != WIFI_EVENT)
		return;

	if (type == WIFI_EVENT_STA_DISCONNECTED) {
		CONNECTED = false;
		ESP_LOGW(TAG, "disconnected");
		esp_wifi_connect();
	}

	if (type == WIFI_EVENT_STA_CONNECTED) {
		CONNECTED = true;
		ESP_LOGI(TAG, "connected");
	}
}

void wifi_init() {
	CONNECTED = false;
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

bool wifi_connnnected() {
    	return CONNECTED;
}
