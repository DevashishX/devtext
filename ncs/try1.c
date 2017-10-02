#include <ncurses.h>
#include <stdio.h>
int main(){
	int maxrow, maxcol;
	initscr();
	curs_set(1);
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	getmaxyx(stdscr, maxrow, maxcol);
	mvprintw(maxrow-1, maxcol-20, "Wassup Devashish !");
	attroff(COLOR_PAIR(1));
	mvprintw(maxrow/2, maxcol/2, "Good Morning !\n");
	move(10, 10);
	refresh();

	getch();
	endwin();

}