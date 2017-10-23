#include<ncurses.h>

int main(){
	initscr();
	noecho();
	raw();
	int ch;
	ch = getch();
	endwin();
	printf("KEY NAME : #%s# - #%d#\n", keyname(ch),ch);
	return 0;

}
