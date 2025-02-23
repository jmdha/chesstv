#pragma once

#include <esp_err.h>
#include <stdbool.h>

void cb_stream_task(void*);
bool cb_stream_board(char* fen, bool* new_game, bool* new_position);
