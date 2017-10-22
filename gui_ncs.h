#ifndef GUI_NCS_H
#define GUI_NCS_H
#include "buffer.h"
#include <ncurses.h>


void curses_init();
void loadwin(buffer *bf, int y);
void tildeall();
#endif