CC=gcc
DEPS = buffer.h gui_ncs.h

devtext: main.c buffer.c gui_ncs.c
	$(CC) -Wall main.c buffer.c gui_ncs.c -o devtext -lncurses


clean:
	rm devtext
