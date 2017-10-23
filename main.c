#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
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
	int srchflag = 0;
	int x = 0, y = 0, offY = 0, ch, xstate = 0, cpyi = 0;
	char str[LINEMAX], rstr[LINEMAX], filename[255], *srch, copybuf[LINEMAX];
	memset(filename, '\0', 255);
	memset(str, '\0', LINEMAX);
	memset(rstr, '\0', LINEMAX);
	memset(copybuf, '\0', LINEMAX);
	buffer *bf, *start, *temp, *temp2, *head;
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
	keypad(stdscr, true);
	/* HELP MENU */
	attron(COLOR_PAIR(1));
	mvprintw(ht/2 - 6, wd/2 - (strlen("WELCOME TO DEVTEXT")/2), "WELCOME TO DEVTEXT");
	attron(COLOR_PAIR(2));
	mvprintw(ht/2 - 5, wd/2 - (strlen("CTRL+S OR F2 : SAVE")/2), "CTRL+S OR F2 : SAVE");
	mvprintw(ht/2 - 4, wd/2 - (strlen("CTRL+A OR F3 : SAVE AND QUIT")/2), "CTRL+A OR F3 : SAVE AND QUIT");
	mvprintw(ht/2 - 3, wd/2 - (strlen("CTRL+Q OR F4 : QUIT WITHOUT SAVE")/2), "CTRL+Q OR F4 : QUIT WITHOUT SAVE");
	mvprintw(ht/2 - 2, wd/2 - (strlen("CTRL+F OR F5 : SEARCH")/2), "CTRL+F OR F5 : SEARCH");
	mvprintw(ht/2 - 1, wd/2 - (strlen("CTRL+R OR F6 : SEARCH AND REPLACE")/2), "CTRL+R OR F6 : SEARCH AND REPLACE");
	mvprintw(ht/2 - 0, wd/2 - (strlen("CTRL+X OR F7 : CUT")/2), "CTRL+X OR F7 : CUT");
	mvprintw(ht/2 + 1, wd/2 - (strlen("CTRL+C OR F8 : COPY")/2), "CTRL+C OR F8 : COPY");
	mvprintw(ht/2 + 2, wd/2 - (strlen("CTRL+V OR F9 : PASTE")/2), "CTRL+V OR F9 : PASTE");
	mvprintw(ht/2 + 3, wd/2 - (strlen("CTRL+H OR F10 : SHOW HELP WINDOW")/2), "CTRL+H OR F10 : SHOW HELP WINDOW");
	mvprintw(ht/2 + 4, wd/2 - (strlen("WINDOW SIZE:     X    ")/2), "WINDOW SIZE: %3d X %3d", wd, ht); 
	mvprintw(ht/2 + 5, wd/2 - (strlen("PRESS ANY KEY TO CONTINUE")/2), "PRESS ANY KEY TO CONTINUE");
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


	//attron(COLOR_PAIR(2));
	start = bf;
	head = bf;
	temp2 = bf;
	loadwin(start, 0);
	move(0, 0);
	while((ch = getch())){

		switch (ch){
			case KEY_UP: //up arrow
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
				else  if( y == 0 && start->prev != NULL && bf->prev != NULL){
					start = start->prev;
					bf = bf->prev;
					loadwin(start, 0);
					move(y, x);
				}
				break;
			case KEY_DOWN: //down arrow
				if(y < ht - 2 && bf != NULL){
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
				else if( y == ht - 2 && bf->next != NULL && start->next != NULL){
					start = start->next;
					bf = bf->next;
					loadwin(start, 0);
					move(y, x);
				}
				break;
			case KEY_LEFT: //left arrow
				if(x > 0){
					move(y, --x);
				}
				break;
			case KEY_RIGHT: //right arrow
				if(x < LINEMAX - 1 && x < bf->num_chars - 1){
					move(y, ++x);
				}
				break;
			case KEY_BACKSPACE: //BACKSPACE key
				bf->curX = x;
				if(bf->prev != NULL){
					temp = bf->prev;
					
				}
				if(x == 0 && y ==0){
					move(0, 0);
				}
				else if(x > 0 && x < bf->num_chars){
					memmove((bf->line + x - 1), (bf->line + x), bf->num_chars - x);
					(bf->num_chars)--;
					x--;
					bf->curX = x;
					loadwin(start, 0);
					move(y, x);
					refresh();


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
					if(y == 0 && bf->prev != NULL && start->prev != NULL){
						start = bf;
						offY--;
						loadwin(start, 0);
						move(y, bf->curX = x = bf->num_chars - x - 1);
					}
					else{
						if(y == 0)
							y = 0;
						else
							y--;
						move(y, bf->curX = x = bf->num_chars - x - 1);
						loadwin(start, 0);	
					}
				}
				break;
			case KEY_DC:
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
				for(int i = 0; i < ht - 2 && temp2->next != NULL; i++){
					temp2 = temp2->next;
				}
				start = temp2;
				bf = temp2;
				y = x = 0;
				move(y, x);
				loadwin(start, 0);
				break;
			case KEY_PPAGE:
				for(int i = 0; i < ht - 2 && temp2->prev != NULL; i++){
					temp2 = temp2->prev;
				}
				start = temp2;
				bf = temp2;
				y = x = 0;
				move(y, x);
				loadwin(start, 0);
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
					bf = bf->next;
					if(y < ht - 2)
						move(++y, x = 0);
					else{
						if(start->next != NULL){
							start = start->next;
							offY++;
						}

						move(y = ht - 2, x = 0);
					}
					loadwin(start, 0);


				}
				else if(x == bf->num_chars || x == bf->num_chars - 1){
					bufInsert(bf);
					clear();
					loadwin(start, 0);					
					bf = bf->next;
					if(y < ht - 2)
						move(++y, x = 0);
					else{
						if(start->next != NULL){
							start = start->next;
							offY++;
						}

						move(y = ht - 2, x = 0);
					}
					loadwin(start, 0);
				}
				break;
			case 6:
			case KEY_F(5): //search
				temp2 = head;
				attron(COLOR_PAIR(1));
				move(ht - 1, 0);
				clrtoeol();
				mvprintw(ht - 1, 0, "Enter Search String: ");
				echo();
				mvscanw(ht - 1, strlen("Enter Search String: "), "%[^\n]s", str);
				refresh();
				noecho();
				while(temp2->next != NULL){
					x = 0;
					while(x >= 0 && x < temp2->num_chars - 1){

						srch = strstr(temp2->line + x, str);
						if(srch != NULL){
							x = temp2->curX = srch - temp2->line;
							bf = temp2;
							attroff(COLOR_PAIR(1));
							if(temp2->cur_line - start->cur_line >= 0 && temp2->cur_line - start->cur_line < ht - 1){
								y = temp2->cur_line - start->cur_line ;
								move(y, x);
							}
							else{
								y = 0;
								start = temp2;
								loadwin(start, 0);
								
							}
							attron(COLOR_PAIR(1));
							move(ht - 1, 0);
							clrtoeol();
							mvprintw(ht - 1, 0, "ENTER : search next ANY: exit search");
							move(y , x);
							refresh();
							if((ch = getch()) != ERR){
								if(ch == '\n'){
									x++;
								}
								else{
									srchflag = 1;
									attroff(COLOR_PAIR(1));
									move(y, x);
									break;
								}
							}

						}
						else{
							x++;
						}
					}
					if(srchflag == 1)
						break;
					temp2 = temp2->next;
				}
				if(srchflag == 0){
					attron(COLOR_PAIR(1));
					move(ht - 1, 0);
					clrtoeol();
					mvprintw(ht - 1, 0, "END OF SEARCH");
					attroff(COLOR_PAIR(1));
					refresh();
					ch = getch();
				}
				srchflag = 0;
				break;
			case 18:
			case KEY_F(6): //search replace
				temp2 = head;
				attron(COLOR_PAIR(1));
				move(ht - 1, 0);
				clrtoeol();
				echo();
				mvprintw(ht - 1, 0, "Enter Search String: ");
				refresh();
				mvscanw(ht - 1, strlen("Enter Search String: "), "%[^\n]s", str);
				clrtoeol();
				mvprintw(ht - 1, 0, "Enter Replace String: ");
				refresh();
				mvscanw(ht - 1, strlen("Enter Replace String: "), "%[^\n]s", rstr);				
				noecho();
				while(temp2->next != NULL){
					x = 0;
					while(x >= 0 && x < temp2->num_chars - 1){

						srch = strstr(temp2->line + x, str);
						if(srch != NULL){
							x = temp2->curX = srch - temp2->line;
							bf = temp2;
							attroff(COLOR_PAIR(1));
							if(temp2->cur_line - start->cur_line >= 0 && temp2->cur_line - start->cur_line < ht - 1){
								y = temp2->cur_line - start->cur_line ;
								move(y, x);
							}
							else{
								y = 0;
								start = temp2;
								loadwin(start, 0);
								
							}
							attron(COLOR_PAIR(1));
							move(ht - 1, 0);
							clrtoeol();
							mvprintw(ht - 1, 0, "ENTER: search next CTRL+R OR F(6): replace ANY: exit");
							move(y , x);
							refresh();
							if((ch = getch())){
								if(ch == '\n'){
									x++;
								}
								else if((ch == KEY_F(6) || ch == 18) && (x + strlen(rstr) - 1 < LINEMAX)){
									for(int i = 0; i < strlen(str); i++){
										memmove((bf->line + x), (bf->line + x + 1), bf->num_chars - x - 1);
										(bf->num_chars)--;
										
									}
									for(int i = 0; i < strlen(rstr); i++){
										lineInsert(bf, x + i, rstr[i]);
									}
									attroff(COLOR_PAIR(1));
									loadwin(start, 0);
									attron(COLOR_PAIR(1));
								}
								else{
									srchflag = 1;
									attroff(COLOR_PAIR(1));
									move(y, x);
									break;
								}
							}

						}
						else{
							x++;
						}
					}
					if(srchflag == 1)
						break;
					temp2 = temp2->next;
				}
				if(srchflag == 0){
					attron(COLOR_PAIR(1));
					move(ht - 1, 0);
					clrtoeol();
					mvprintw(ht - 1, 0, "END OF SEARCH");
					attroff(COLOR_PAIR(1));
					refresh();
					ch = getch();
				}
				srchflag = 0;
				
				break;

			case 24:
			case KEY_F(7): //cut ctrl+X
				attron(COLOR_PAIR(1));
				move(ht - 1, 0);
				clrtoeol();
				echo();
				mvprintw(ht - 1, 0, "RIGHT: select LEFT: deselect CTRL+X: cut selection ANY: cancel");
				attroff(COLOR_PAIR(1));
				refresh();
				noecho();
				move(y, x);
				cpyi = 0;
				xstate = x;
				memset(copybuf, '\0', LINEMAX);
				while((ch = getch()) != ERR){
					if(ch == KEY_RIGHT && x < LINEMAX - 1 && x < bf->num_chars - 1){
						copybuf[cpyi++] = bf->line[x];
						addch(bf->line[x] | A_STANDOUT);
						move(y, x++);
					}
					else if((ch == KEY_LEFT) && x >= 0 && x > xstate){
						
						move(y, --x);						
						copybuf[--cpyi] = '\0';
						addch(bf->line[x] | A_NORMAL);

					}
					else if(ch == 24 || ch == KEY_F(7)){
						x = xstate;
						for(int i = 0; i < strlen(copybuf); i++){
							memmove((bf->line + x), (bf->line + x + 1), bf->num_chars - x - 1);
							(bf->num_chars)--;
										
						}
						loadwin(start, 0);
						move(y, x);
						break;
					}
					else {
						loadwin(start, 0);
						memset(copybuf, '\0', LINEMAX);
						move(y, x = xstate);
						break;

					}
				move(ht - 1, 0);
				clrtoeol();
				attron(COLOR_PAIR(1));
				mvprintw(ht - 1, 0, " \"%s\" | RIGHT: select LEFT: deselect CTRL+X: cut selection ANY: cancel", copybuf);
				attroff(COLOR_PAIR(1));
				refresh();
				move(y, x);
				}			

				move(y, x);

				break;
			case 3:
			case KEY_F(8): //copy ctrl+C
				attron(COLOR_PAIR(1));
				move(ht - 1, 0);
				clrtoeol();
				echo();
				mvprintw(ht - 1, 0, "RIGHT: select LEFT: deselect CTRL+C: copy selection ANY: cancel");
				attroff(COLOR_PAIR(1));
				refresh();
				noecho();
				move(y, x);
				cpyi = 0;
				xstate = x;
				memset(copybuf, '\0', LINEMAX);
				while((ch = getch()) != ERR){
					if(ch == KEY_RIGHT && x < LINEMAX - 1 && x < bf->num_chars - 1){
						copybuf[cpyi++] = bf->line[x];
						addch(bf->line[x] | A_STANDOUT);
						move(y, x++);
					}
					else if((ch == KEY_LEFT) && x >= 0 && x > xstate){
						
						move(y, --x);						
						copybuf[--cpyi] = '\0';
						addch(bf->line[x] | A_NORMAL);

					}
					else if(ch == 3 || ch == KEY_F(8)){
						x = xstate;
						loadwin(start, 0);
						move(y, x);
						break;
					}
					else {
						loadwin(start, 0);
						memset(copybuf, '\0', LINEMAX);
						move(y, x = xstate);
						break;


					}
				move(ht - 1, 0);
				clrtoeol();
				attron(COLOR_PAIR(1));
				mvprintw(ht - 1, 0, " \"%s\" | RIGHT: select LEFT: deselect CTRL+C: copy selection ANY: cancel", copybuf);
				attroff(COLOR_PAIR(1));
				refresh();
				move(y, x);
				}				
				move(y, x);

				break;
			case 22:
			case KEY_F(9): //paste ctrl+V
				if(strlen(copybuf) != 0 && copybuf[0] != '\0' && x + strlen(copybuf) - 1 < LINEMAX){
					for(int i = 0; i < strlen(copybuf); i++){
						lineInsert(bf, x + i, copybuf[i]);
					}
					
				}
				move(y, bf->curX = x = x + strlen(copybuf));
				loadwin(start, 0);

			
				break;
			case 19: // ctrl+S
			case KEY_F(2): //save
				move(ht -1, 0);
				clrtoeol();
				if(newfl == 1){
					attron(COLOR_PAIR(1));
					mvprintw(ht - 1, 0, "Enter file Name: ");
					echo();
					mvscanw(ht - 1, strlen("Enter file Name: "), "%[^\n]s", filename);
					noecho();
					attroff(COLOR_PAIR(1));
					refresh();
					if((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1){
						perror("Save Error: ");
						return 0;
					}
					newfl = 0;
					loadwin(start, 0);
					move(y, x);

				}
				else{
					fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
				}
				bufSave(fd, head);

				break;
			case 1:
			case KEY_F(3): //save and quit
				move(ht - 1, 0);
				clrtoeol();
				if(newfl == 1){
					attron(COLOR_PAIR(1));
					mvprintw(ht - 1, 0, "Enter File name: ");
					echo();
					mvscanw(ht - 1, strlen("Enter File name: "), "%[^\n]s", filename);
					noecho();
					attroff(COLOR_PAIR(1));
					if((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1){
						perror("Save Error: ");
						return 0;
					}
					refresh();
					loadwin(start, 0);
					move(y, x);

				}
				else{
					fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
				}
				bufSave(fd, head);
			case 17:
			case KEY_F(4):
				endwin();
				close(fd);
				printf("%s\n", filename);
				bufPrintAll(head);
				bufDestroy(head);
				return 0;
				break;
			case 8:
			case KEY_F(10):
				clear();
				attron(COLOR_PAIR(1));
				mvprintw(ht/2 - 6, wd/2 - (strlen("WELCOME TO DEVTEXT")/2), "WELCOME TO DEVTEXT");
				attron(COLOR_PAIR(2));
				mvprintw(ht/2 - 5, wd/2 - (strlen("CTRL+S OR F2 : SAVE")/2), "CTRL+S OR F2 : SAVE");
				mvprintw(ht/2 - 4, wd/2 - (strlen("CTRL+A OR F3 : SAVE AND QUIT")/2), "CTRL+A OR F3 : SAVE AND QUIT");
				mvprintw(ht/2 - 3, wd/2 - (strlen("CTRL+Q OR F4 : QUIT WITHOUT SAVE")/2), "CTRL+Q OR F4 : QUIT WITHOUT SAVE");
				mvprintw(ht/2 - 2, wd/2 - (strlen("CTRL+F OR F5 : SEARCH")/2), "CTRL+F OR F5 : SEARCH");
				mvprintw(ht/2 - 1, wd/2 - (strlen("CTRL+R OR F6 : SEARCH AND REPLACE")/2), "CTRL+R OR F6 : SEARCH AND REPLACE");
				mvprintw(ht/2 - 0, wd/2 - (strlen("CTRL+X OR F7 : CUT")/2), "CTRL+X OR F7 : CUT");
				mvprintw(ht/2 + 1, wd/2 - (strlen("CTRL+C OR F8 : COPY")/2), "CTRL+C OR F8 : COPY");
				mvprintw(ht/2 + 2, wd/2 - (strlen("CTRL+V OR F9 : PASTE")/2), "CTRL+V OR F9 : PASTE");
				mvprintw(ht/2 + 3, wd/2 - (strlen("CTRL+H OR F10 : SHOW HELP WINDOW")/2), "CTRL+H OR F10 : SHOW HELP WINDOW");
				mvprintw(ht/2 + 4, wd/2 - (strlen("WINDOW SIZE:     X    ")/2), "WINDOW SIZE: %3d X %3d", wd, ht);
				mvprintw(ht/2 + 5, wd/2 - (strlen("PRESS ANY KEY TO CONTINUE")/2), "PRESS ANY KEY TO CONTINUE");
				attroff(COLOR_PAIR(3));
				move(y, x);
				curs_set(0);
				getch();
				curs_set(2);
				loadwin(start, 0);
				break;
			default:
				if(x >= 0 && x < LINEMAX && bf->num_chars != LINEMAX){
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
					clear();
					loadwin(start, 0);
					refresh();
					if(bf->num_chars == LINEMAX - 1 || x == LINEMAX - 1){
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
				break;



		}
		attron(COLOR_PAIR(1));
		move(ht -1, 0);
		clrtoeol();
		/*mvprintw(ht - 1, 0, "%s | row : %3d | col: %3d", filename, y, x);*/
		mvprintw(ht - 1, 0, "%s | row : %3d | cl: %3d | col: %3d | nc: %3d | cp: \"%s\" |", filename, y, bf->cur_line, x, bf->num_chars , copybuf);
		move(y, x);
		attroff(COLOR_PAIR(1));
		refresh();


	}

	endwin();
	close(fd);
	bufDestroy(head);
	return 0;
}