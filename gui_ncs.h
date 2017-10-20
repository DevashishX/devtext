#ifndef GUI_NCS_H
#define GUI_NCS_H
#include "buffer.h"
#include <ncurses.h>



#define warr 5

void curses_init();
void loadscreen(WINDOW *win[warr], buffer *bf);
void keyhit(WINDOW *win, buffer *bf, int ch, int x, int y);
void list(WINDOW *bar, buffer *bf, int ch);
void loadscreen(WINDOW *win[warr], buffer *bf);


#endif