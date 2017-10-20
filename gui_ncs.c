#include "gui_ncs.h"
#include <ncurses.h>




void curses_init(){
    initscr();
    noecho();                   
    cbreak();
    curs_set(2);
    start_color();
    //scrollok(stdscr, TRUE);
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_WHITE);


}
