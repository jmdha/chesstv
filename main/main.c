#include <esp_wifi.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <string.h>

#define SSID CONFIG_ESP_WIFI_SSID
#define PASS CONFIG_ESP_WIFI_PASSWORD

void init_nvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
}

void init_wifi() {
    ESP_ERROR_CHECK(esp_netif_init());

    wifi_init_config_t config_init = WIFI_INIT_CONFIG_DEFAULT();
    printf("run esp_wifi_init\n");
    ESP_ERROR_CHECK(esp_wifi_init(&config_init));
    wifi_config_t config_wifi = {
        .sta = {
            .ssid               = SSID,
            .password           = PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK
        }
    };
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
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    init_nvs();
    init_wifi();
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
