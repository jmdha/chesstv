#include <ctype.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <string.h>
#include <driver/gpio.h>

#include "controller.h"
#include "position.h"
#include "stream.h"

#define SER   GPIO_NUM_33
#define RCLK  GPIO_NUM_25
#define SRCLK GPIO_NUM_26

void cb_board_init(void) {
	gpio_set_direction(SER,   GPIO_MODE_OUTPUT);
	gpio_set_direction(SRCLK, GPIO_MODE_OUTPUT);
	gpio_set_direction(RCLK,  GPIO_MODE_OUTPUT);
}

void ctv_print(const position_t* pos) {
    char buf[128];
    int i = 0;
    for (int y = 0; y < 8; y++) {
        buf[i++] = '\n';
        for (int x = 0; x < 8; x++) {
            if (position_is_set(pos, x, y))
                buf[i++] = 'X';
            else
                buf[i++] = ' ';
        }
    }
    ESP_LOGI("ctv_display", "%.*s", 80, buf);
}

void cb_board_display(const position_t* pos) {
	for (int p = 0; p < 8; p++) {
		for (int p_ = 0; p_ < 8; p_++) {
			gpio_set_level(SER, p == p_);
			gpio_set_level(SRCLK, 0);
			gpio_set_level(SRCLK, 1);
		}
		for (int n = 0; n < 8; n++) {
			bool val = position_is_set(pos, p, n);
			gpio_set_level(SER, !val);
			gpio_set_level(SRCLK, 0);
			gpio_set_level(SRCLK, 1);
		}
		gpio_set_level(RCLK, 0);
		gpio_set_level(RCLK, 1);
	}

}

void cb_board_update(void*) {
  position_t* pos = position();
  while (true) {
      char fen[128];
      bool new_game;
      bool new_position;
      if (cb_stream_board(fen, &new_game, &new_position) && new_position)
        position_load(pos, fen);
      if (pos)
	cb_board_display(pos);
  }
  position_free(pos);
}
