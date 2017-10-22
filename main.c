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

int fileexist(char const *path){
     int fd = open(path, O_RDWR );   // open file in read mode
     if(fd == -1)                   // couldn't open mean file not existing
        return 0;
     close(fd);   //close file if opened means file exists
     return 1;
}

int main(int argc, char const *argv[]){
	int fd, newfl = 0;
	int ht, wd;
	int nolines = 0;
	int x = 0, y = 0, offX = 0, offY = 0, ch;
	char str[LINEMAX], filename[255];
	memset(filename, '\0', 255);
	buffer *bf, *start, *temp;
	bf = (buffer *)malloc(sizeof(buffer));
	bufInit(bf);
	if(argc == 2){
		strcpy(filename, argv[1]);
		if(fileexist(argv[1])){
			fd = open(argv[1], O_RDWR );
			bufLoad(fd, bf);
			close(fd);
			
		}
		else{
			newfl = 2;
			bf->line[0]='\n';
			bf->num_chars = 1;

		}
	}
	else if(argc == 1){

		newfl = 1;
		bf->line[0]='\n';
		bf->num_chars = 1;
	}
	else{
		printf("USAGE: ./devtext <filename> or ./devtext\n");
		bufDestroy(bf);
		free(bf);
		return 0;
	}



/* #################################### */
	curses_init();
	//idlok(stdscr, TRUE);
/* #################################### */
	getmaxyx(stdscr, ht, wd);
	WINDOW *local;
	local = newwin(ht, wd, 0, 0);
	keypad(stdscr, true);
	/* HELP MENU */
	attron(COLOR_PAIR(1));
	mvprintw(ht/2 - 3, wd/2 - (strlen("WELCOME TO DEVTEXT")/2), "WELCOME TO DEVTEXT");
	attron(COLOR_PAIR(2));
	mvprintw(ht/2 - 2, wd/2 - (strlen("F2: SAVE F3: SEARCH")/2), "F2: SAVE F3: SEARCH");
	mvprintw(ht/2 - 1, wd/2 - (strlen("F4: SEARCH & REPLACE")/2), "F4: SEARCH & REPLACE");
	mvprintw(ht/2 + 0, wd/2 - (strlen("F5: SAVE & QUIT F10: QUIT NOSAVE")/2), "F5: SAVE & QUIT F10: QUIT NOSAVE");
	mvprintw(ht/2 + 1, wd/2 - (strlen("WINDOW SIZE:     X    ")/2), "WINDOW SIZE: %3d X %3d", ht, wd); 
	mvprintw(ht/2 + 2, wd/2 - (strlen("PRESS ANY KEY TO CONTINUE")/2), "PRESS ANY KEY TO CONTINUE", ht, wd);
	attroff(COLOR_PAIR(3));
	y = 0, x = 0;
	move(y, x);
	refresh();
	getch();
	clear();

	attron(COLOR_PAIR(1));
	mvprintw(ht - 1, 0, "row : %3d | cl: %3d | col: %3d | nc: %3d ", y, bf->cur_line, x, bf->num_chars );
	move(y, x);
	attroff(COLOR_PAIR(1));
	refresh();



	start = bf;
	loadwin(start, 0);
	move(0, 0);
	while((ch = getch())){

		switch (ch){
			case KEY_UP:
				if(y > 0 && bf != NULL && bf->prev != NULL){
					bf = bf->prev;
					if(x >= bf->num_chars){
						x = bf->num_chars -1;	
						move(--y, x);
					}
					else{
						move(--y, x);
					}
				}
				break;
			case KEY_DOWN:
				if(y < ht - 1 && bf != NULL){
					if(bf->next != NULL){
						bf = bf->next;
						if(x >= bf->num_chars){
							if(bf->num_chars  > 0){
								x = bf->num_chars - 1;								
							}
							else{
								x = 0;
							}
							move(++y, x);
						}
						else{
							move(++y, x);
						}
					}
				}
				break;
			case KEY_LEFT:
				if(x > 0){
					move(y, --x);
				}
				break;
			case KEY_RIGHT:
				if(x < LINEMAX - 1 && x < bf->num_chars - 1){
					move(y, ++x);
				}
				break;
			case KEY_BACKSPACE: //BACKSPACE IS HERE
				bf->curX = x;
				temp = bf->prev;
				if(x == 0 && y ==0){
					move(0, 0);
				}
				else if(x > 0 && x < bf->num_chars){
					//mvdelch(y, x - 1);
					memmove((bf->line + x - 1), (bf->line + x), bf->num_chars - x);
					(bf->num_chars)--;
					x--;
					bf->curX = x;
					//clear();
					move(y, 0);
					clrtoeol();
					mvprintw(y, 0, "%s", bf->line);
					loadwin(start, 0);
					move(y, x);


				}
				else if(x == 0){
					if(bf->prev != NULL && bf->prev->num_chars == 0){
						bf->prev->line[0] = '\n';
						bf->prev->num_chars = 1;
					}
					memmove((bf->prev->line + bf->prev->num_chars - 1), bf->line, (bf->num_chars - 0));
					bf->prev->next = bf->next;
					if(bf->next != NULL){
						bf->next->prev = bf->prev;						
					}
					x = bf->num_chars - 1;
					if(bf->prev->num_chars == 1 && bf->num_chars == 1){
						bf->prev->num_chars = 1;
					}
					else{
						bf->prev->num_chars = bf->prev->num_chars + bf->num_chars - 1;
					}
			
					free(bf->line);
					free(bf);
					bf = temp;
					if(bf->next != NULL){
						bufDecr(bf->next, 1);
						
					}
					clear();
					loadwin(start, 0);	
					move(--y, bf->curX = x = bf->num_chars - x - 1);
				}

				/*if(x > 0 && x < LINEMAX - 1 && x < bf->num_chars - 1){
					lineRemove(bf, y, bf->curX);
					//mvclearline(y, 0);
					mvdelch(y, x);
					refresh();
					//mvprintw(y, 0, "%s", bf->line);
					move(y, --x);
				}
				else if(x == 0 && y > 0){
					temp = bf->prev;

					memmove((bf->prev->line + bf->prev->num_chars - 1), (bf->line), bf->num_chars);
					bf->next->prev = bf->prev;
					bf->prev->next = bf->next;
					bufDecr(bf->next, 1);
					bf->prev->curX = bf->prev->num_chars;
					bf->prev->num_chars = bf->prev->num_chars + bf->num_chars - 1;
					move(--y, x = bf->prev->curX);
					free(bf->line);
					free(bf);
					bf = temp;
					clear();
					loadwin(start, 0);
					refresh();					
				}
				else if(x == 0 && y == 0){
					move(0, 0);
				}
				else if(x == bf->num_chars - 1){
					x--;
				}*/
				break;
			case KEY_HOME:
				x = 0;
				bf->curX = x;
				move(y, bf->curX);
				break;
			case KEY_END:
				if(bf->num_chars > 0){
					x = bf->num_chars - 1;					
				}
				else{
					x = 0;
				}
				bf->curX = x;
				move(y, bf->curX);
				break;
			case KEY_NPAGE:
				break;
			case KEY_PPAGE:
				break;
			case '\n':
				bf->curX = x;
				lineInsert(bf, bf->curX, ch);
				if(x < bf->num_chars -1){
					bufInsert(bf);
					memmove(bf->next->line, (bf->line + x + 1), bf->num_chars - x + 1);
					memset((bf->line + x + 1), '\0', bf->num_chars - x);
					bf->next->num_chars = bf->num_chars - x - 1;
					bf->num_chars = x + 1;
					clear();
					loadwin(start, 0);
					bf = bf->next;
					move(++y, x = 0);


				}
				else if(x == bf->num_chars || x == bf->num_chars - 1){
					bufInsert(bf);
					clear();
					loadwin(start, 0);					
					bf = bf->next;
					move(++y, x = 0);
				}
				//addch('\n');
				//bufInsert(bf);
				//bf = bf->next;
				//move(++y, x = 0);	
				/*clear();
				loadwin(start, 0);
				move(++y, x = 0);*/
				/*while(x < LINEMAX){
					mvaddch(y, x, ' ');
					x++;
				}*/
				
				//loadwin(bf->next, y + 1);

				
				break;
			case KEY_F(2): //save
				clrtoeol(ht - 1, 0);
				if(newfl == 1){
					attron(COLOR_PAIR(1));
					mvprintw(ht - 1, 0, "Enter file Name: ");
					echo();
					mvscanw(ht - 1, strlen("Enter file Name: "), "%[^\n]s", filename);
					noecho();
					attroff(COLOR_PAIR(1));
					fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
					newfl = 0;
					clear();
					if(start->prev != NULL)
						start = start->prev;
					attron(COLOR_PAIR(1));
					mvprintw(ht - 1, 0, "row : %3d   |   col: %3d", y, x);
					attroff(COLOR_PAIR(1));
					refresh();
					loadwin(start, 0);
					move(y, x);

				}
				else{
					fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
				}
				bufSave(fd, start);
				break;
			case KEY_F(3): //search
				break;
			case KEY_F(4): //search replace
				break;
			case KEY_F(5): //save and quit
				mvclearline(ht - 1, 0);
				if(newfl == 1){
					attron(COLOR_PAIR(1));
					mvprintw(ht - 1, 0, "Enter File name: ");
					echo();
					mvscanw(ht - 1, strlen("Enter File name: "), "%[^\n]s", filename);
					noecho();
					attroff(COLOR_PAIR(1));
					fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
					newfl = 0;
					clear();
					if(start->prev != NULL)
						start = start->prev;
					attron(COLOR_PAIR(1));
					mvprintw(ht - 1, 0, "row : %3d   |   col: %3d", y, x);
					attroff(COLOR_PAIR(1));
					refresh();
					loadwin(start, 0);
					move(y, x);

				}
				else{
					fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
				}
				bufSave(fd, start);
			case KEY_F(10):
				delwin(local);
				endwin();
				close(fd);
				printf("%s\n", filename);
				bufPrintAll(start);
				bufDestroy(start);
				return 0;
				break;
			default:
				if(x >= 0 && x < LINEMAX){
					//mvaddch(y, x, ch);
					bf->curX = x;
					if(x < LINEMAX - 4 && ch == '\t'){
						lineInsert(bf, bf->curX, ' ');
						lineInsert(bf, bf->curX, ' ');
						lineInsert(bf, bf->curX, ' ');	
						lineInsert(bf, bf->curX, ' ');
					}
					else{
						if(ch == '\t')
							ch = ' ';
						lineInsert(bf, bf->curX, ch);						
					}
					mvprintw(y, 0, "%s", bf->line);
					if(x == LINEMAX - 1){
						x = LINEMAX - 1;
					}
					else {
						if(x < LINEMAX - 4 && ch == '\t')
							x = x + 4;
						else
							x++;
					}
					
				}
				else {
					x = LINEMAX - 1;
				}
				/*bf->curX = x;
				bf->line[curX]*/
				break;



		}
		attron(COLOR_PAIR(1));
		mvprintw(ht - 1, 0, "row : %3d | cl: %3d | col: %3d | nc: %3d ", y, bf->cur_line, x, bf->num_chars );
		move(y, x);
		attroff(COLOR_PAIR(1));
		refresh();


	}





	delwin(local);
	endwin();
	close(fd);
	bufDestroy(bf);
	return 0;
}