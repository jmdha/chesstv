#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include <freertos/event_groups.h>

#include "wifi.h"

#define SSID CONFIG_ESP_WIFI_SSID
#define PASS CONFIG_ESP_WIFI_PASSWORD
#define AUTH WIFI_AUTH_WPA2_PSK

static const char *TAG = "wifi";

bool CONNECTED;
EventGroupHandle_t EVENT_GROUP;

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

wifi_init_config_t generate_init_config() {
	wifi_init_config_t c = WIFI_INIT_CONFIG_DEFAULT();
	return c;
}

wifi_config_t generate_config() {
	wifi_config_t c = {
		.sta = {
			.ssid               = SSID,
			.password           = PASS,
			.threshold.authmode = AUTH
		}
	};
	return c;
}

void wifi_init() {
	wifi_init_config_t           config_init;
	wifi_config_t                config_wifi;
	esp_event_handler_instance_t event_instance;

	CONNECTED   = false;
  	EVENT_GROUP = xEventGroupCreate();
	config_init = generate_init_config();
	config_wifi = generate_config();

	esp_netif_init();
	esp_event_loop_create_default();
	esp_netif_create_default_wifi_sta();
	esp_wifi_init(&config_init);
	esp_wifi_set_mode(WIFI_MODE_STA);
	esp_wifi_set_config(WIFI_IF_STA, &config_wifi);
	esp_event_handler_instance_register(
		WIFI_EVENT,
		ESP_EVENT_ANY_ID,
		&event_handler,
		NULL,
		&event_instance
	);

	esp_wifi_start();
	esp_wifi_connect();
}

bool wifi_connnnected() {
    	return CONNECTED;
}
