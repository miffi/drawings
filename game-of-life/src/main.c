#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include <unistd.h>

#include <termbox.h>

#define LOC(x, y) (y * width + x)

void show_iteration(bool* reel);
void next(bool* reel);

int main() {
    if(tb_init() < 0) {
        perror("Error: Could not start termbox");
        exit(1);
    }
    tb_clear();
    tb_select_input_mode(TB_INPUT_MOUSE);

    srand(time(NULL));

    uint32_t width = tb_width(), height = tb_height();
    bool *reel = malloc(width * height * sizeof(bool));
    //for(size_t i = 0; i < width * height; i++) {
    //    reel[i] = rand() % 2;
    //}
    memset(reel, 0, width*height*sizeof(bool));

    struct tb_event event;
    bool pause = true;
    while(true) {
        show_iteration(reel);
        if(!pause) {
            next(reel);
        }

        if(pause) {
            tb_poll_event(&event);
        } else if(tb_peek_event(&event, 100) <= 0) {
            continue;
        }

        if(event.type == TB_EVENT_KEY) {
            if(event.ch == 'q') {
                break;
            }
            if(event.ch == 'p') {
                pause = !pause;
            }
            if(event.ch == 'c') {
                memset(reel, 0, width*height*sizeof(bool));
                pause = true;
            }
            if(event.ch == 'r') {
                for(size_t i = 0; i < width * height; i++) {
                    reel[i] = rand() % 2;
                }
            }
            if(event.key == TB_KEY_SPACE) {
                next(reel);
            }
        } else if(event.type == TB_EVENT_MOUSE && event.key == TB_KEY_MOUSE_LEFT) {
            bool *x = &reel[LOC(event.x, event.y)];
            *x = !(*x);
        }
    }

    free(reel);
    tb_shutdown();
}

void show_iteration(bool* reel) {
    uint32_t width = tb_width(), height = tb_height();
    struct tb_cell *t = tb_cell_buffer();
    for(size_t i = 0; i < width * height; i++) {
        t[i].ch = reel[i] ? '#': ' ';
        t[i].fg = i % 8 == 1 ? 0: i % 8;
    }
    tb_present();
}


void next(bool* reel) {
    int32_t width = tb_width(), height = tb_height();
    bool *buffer = malloc(width * height * sizeof(bool));

    for(int32_t x = 0; x < width; x++) {
        for(int32_t y = 0; y < height; y++) {
            uint8_t count = 0;
            size_t xleft = x + 1 >= width ? 0 : x + 1,
                   xright = x - 1 < 0 ? width - 1: x - 1,
                   ydown = y + 1 >= height ? 0 : y + 1,
                   yup = y - 1 < 0 ? height - 1: y - 1;

            count += reel[LOC(xleft, y)];
            count += reel[LOC(xright, y)];

            count += reel[LOC(x, yup)];
            count += reel[LOC(x, ydown)];

            count += reel[LOC(xleft, ydown)];
            count += reel[LOC(xright, ydown)];

            count += reel[LOC(xleft, yup)];
            count += reel[LOC(xright, yup)];

            if(reel[LOC(x, y)]) {
                buffer[LOC(x, y)] = count == 2 || count == 3;
            } else {
                buffer[LOC(x, y)] = count == 3;
            }
        }
    }
    memcpy(reel, buffer, width * height * sizeof(bool));
    free(buffer);
}
