#include "gui_ncs.h"
#include <ncurses.h>




void curses_init(){
    initscr();
    noecho();                   
    raw();
    curs_set(2);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);



}


void loadwin(buffer *bf, int y){
	clear();
	attron(COLOR_PAIR(3));
	int x = 0;
	while(bf != NULL){
		mvprintw(y, x, "%s", bf->line);
		y++;
		bf = bf->next;
	}

	refresh();
}


void tildeall(){
	int x = 0, y = 0, ht;
	getmaxyx(stdscr, ht, x);
	x = 0;
	for(y = 0; y < ht; y ++){
		mvaddch(y, x, '~');
	}
	refresh();
	move(0, 0);
}
