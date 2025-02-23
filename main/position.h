#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct position_t position_t;

position_t* position();
void position_load(position_t* pos, const char* fen);
void position_free(position_t* pos);
bool position_is_set(const position_t* pos, int x, int y);
void position_set(position_t* pos, int x, int y);
