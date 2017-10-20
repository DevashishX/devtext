#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <ncurses.h>

#include "gui_ncs.h"
#include "buffer.h"

int main(int argc, char const *argv[]){
	int fd, newfl = 0;
	int ht, wd;
	int nolines = 0;
	int x = 0, y = 0, offX = 0, offY = 0, ch;
	buffer *bf;
	bf = (buffer *)malloc(sizeof(buffer));
	bufInit(bf);
	if(argc == 2){
		fd = open(argv[1], O_RDWR | O_CREAT , S_IRWXU);
		bufLoad(fd, bf);
	}
	else if(argc == 1){
		fd = open(".untitled.txt", O_RDWR | O_CREAT , S_IRWXU);
		newfl = 1;
	}
	else{
		printf("USAGE: ./devtext <filename> or ./devtext\n");
		free(bf);
		exit(0);
	}

/* #################################### */
	curses_init();
	idlok(stdscr, TRUE);
/* #################################### */
	getmaxyx(stdscr, ht, wd);
	WINDOW *local;
	local = newwin(ht, wd, 0, 0);
	keypad(stdscr, true);
	/* HELP MENU */
	attron(COLOR_PAIR(1));
	mvprintw(ht/2 - 2, wd/2 - (strlen("WELCOME TO DEVTEXT")/2), "WELCOME TO DEVTEXT");
	attron(COLOR_PAIR(2));
	mvprintw(ht/2 - 1, wd/2 - (strlen("F2: SAVE F3: SEARCH")/2), "F2: SAVE F3: SEARCH");
	mvprintw(ht/2 - 0, wd/2 - (strlen("F4: SEARCH & REPLACE")/2), "F4: SEARCH & REPLACE");
	mvprintw(ht/2 + 1, wd/2 - (strlen("F5: QUIT F10: QUIT_ABRUPT")/2), "F5: QUIT F10: QUIT_ABRUPT ");
	mvprintw(ht/2 + 2, wd/2 - (strlen("WINDOW SIZE:     X    ")/2), "WINDOW SIZE: %3d X %3d", ht, wd); 
	mvprintw(ht/2 + 3, wd/2 - (strlen("PRESS ANY KEY TO CONTINUE")/2), "PRESS ANY KEY TO CONTINUE", ht, wd);
	attroff(COLOR_PAIR(3));
	y = 0, x = 0;
	move(y, x);
	refresh();
	getch();
	clear();
	mvprintw(ht - 1, 0, "row : %3d   |   col: %3d", y, x);
	move(y, x);
	refresh();

	while((ch = getch())){

		switch (ch){
			case KEY_UP:
				if(y > 0)
					move(--y, x);
				break;
			case KEY_DOWN:
				if(y < ht - 1)
					move(++y, x);
				break;
			case KEY_LEFT:
				if(x > 0)
					move(y, --x);
				break;
			case KEY_RIGHT:
				if(x < wd)
					move(y, ++x);
				break;
			case KEY_BACKSPACE:
				if(x > 0){
					addch(' ');					
					move(y, --x);

				}
				else if(x == 0){
					addch(' ');
					if(y > 0)
						move(--y, x);
				}
				
					
				break;
			case KEY_NPAGE:
				break;
			case KEY_PPAGE:
				break;
			case '\n':
				addch('\n');
				move(++y, x = 0);	
				
				break;
			case KEY_F(2):
				break;
			case KEY_F(3):
				break;
			case KEY_F(4):
				break;
			case KEY_F(5):
				break;
			case KEY_F(10):
				delwin(local);
				endwin();
				close(fd);
				exit(0);
				break;
			default:
				mvaddch(y, x, ch);
				x++;
				break;



		}
		mvprintw(ht - 1, 0, "row : %3d   |   col: %3d", y, x);
		move(y, x);
		refresh();

		//mvaddch(y, x, ch);
	}





	delwin(local);
	endwin();
	close(fd);
	return 0;
}