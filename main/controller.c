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

void cb_board_light(int x, int y) {
	for (int i = 0; i < 8; i++) {
		const bool val = i == x;
		gpio_set_level(SER, val);
		gpio_set_level(SRCLK, 0);
		gpio_set_level(SRCLK, 1);
	}
	for (int i = 0; i < 8; i++) {
		const bool val = i != y;
		gpio_set_level(SER, val);
		gpio_set_level(SRCLK, 0);
		gpio_set_level(SRCLK, 1);
	}
}

void cb_board_display(const position_t* pos) {
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++)
			if (position_is_set(pos, x, y)) {
				cb_board_light(x, y);
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
