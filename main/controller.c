#include <ctype.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <string.h>

#include "controller.h"
#include "position.h"
#include "stream.h"

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

void cb_board_update(void*) {
  position_t* pos = position();
  while (true) {
      char fen[128];
      bool new_game;
      bool new_position;
      if (cb_stream_board(fen, &new_game, &new_position) && new_position)
        position_load(pos, fen);
      //if (pos)
      //  ctv_display(pos);
  }
  position_free(pos);
}
