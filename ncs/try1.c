#include<ncurses.h>

int main(int argc, char const *argv[]){
	int height, width, ch;
	int x = 0, y = 0;
	WINDOW *local;
	

	initscr();
	getmaxyx(stdscr, height, width);

	local = newwin(height, width, 0, 0);

	start_color();
 	init_pair(1, COLOR_BLACK, COLOR_CYAN);
 	//wattron(local, COLOR_PAIR(1));
	//noecho();
	cbreak();
	keypad(local, TRUE);
	curs_set(2);
	wprintw(local, "HI THERE");
	//x = y = 15;
	//wmove(local, x, y);
	mvwprintw(local, height-1, 0, "USAGE:");
	wmove(local, 0, 0);
	wrefresh(local);
	while(1){
		ch = wgetch(local);
		switch (ch){
			case KEY_UP:
				if(y != 0)
					wmove(local, --y, x);
				break;
			case KEY_DOWN:
				if(y != height -1)
					wmove(local, ++y, x);
				break;
			case KEY_LEFT:
				if(x != 0)
					wmove(local, y, --x);
				break;
			case KEY_RIGHT:
				if(x != width - 1)
					wmove(local, y, ++x);
				break;
			default:
				break;
		}
		if(ch == KEY_F(10)){
			break;
		}

		wrefresh(local);
	}
	


	delwin(local);
	endwin();
	return 0;
}