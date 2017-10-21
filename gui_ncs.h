#ifndef GUI_NCS_H
#define GUI_NCS_H
#include "buffer.h"
#include <ncurses.h>



#define warr 5

void mvclearline(int y, int x);
void curses_init();
void loadwin(buffer *bf, int y);

#endif