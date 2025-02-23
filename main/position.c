#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "position.h"
#include "esp_log.h"

// PERF: This uses a lot of memory to store what is realistically 64 bits of data
struct position_t {
    bool squares[8][8];
};

position_t* position() {
    position_t* pos = malloc(sizeof(position_t));
    return pos;
}

void position_load(position_t* pos, const char* fen) {
    memset(pos, 0, sizeof(position_t));
    for (int y = 7; y >= 0; y--) {
        for (int x = 7; x >= 0;) {
            unsigned char c = *fen;
            if (isdigit(c))
                x -= c - 48;
            else {
                position_set(pos, x, y);
                x -= 1;
            }
            fen++;
        }
        fen++;
    }
}

void position_free(position_t* pos) {
    free(pos);
}

bool position_is_set(const position_t* pos, int x, int y) {
    return pos->squares[x][y];
}

void position_set(position_t* pos, int x, int y) {
    pos->squares[x][y] = true;
}
