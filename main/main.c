#include <freertos/FreeRTOS.h>
#include <nvs_flash.h>

#include "controller.h"
#include "wifi.h"
#include "stream.h"

void init_nvs() {
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
	    ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
}

void app_main(void) {
	init_nvs();
	cb_board_init();
	cb_wifi_init();
	xTaskCreate(cb_stream_task,  "stream", 4096, NULL, 1, NULL);
	xTaskCreate(cb_board_update, "board",  4096, NULL, 2, NULL);
}
