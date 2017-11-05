#include "gui_ncs.h"
#include <ncurses.h>




void curses_init(){
    initscr();
    noecho();                   
    raw();
    curs_set(2);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK); /*COLOR codes for ncurses*/
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);



}

/*Renders the window from the first line i.e. Y = 0, and the "start" buffer pointer.*/
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


/*not used, was initially supposed to put tildes on each line, but later discarded.*/
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
