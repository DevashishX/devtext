#ifndef GUI_NCS_H
#define GUI_NCS_H
#include "buffer.h"
#include <ncurses.h>

void mvclearline(int y, int x);
void curses_init();
void loadwin(buffer *bf, int y);
void tildeall();
#endif