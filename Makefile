CC=gcc
DEPS = buffer.h gui_ncs.h

devtext: main.c buffer.c gui_ncs.c buffer.h gui_ncs.h
	$(CC) -Wall main.c buffer.c gui_ncs.c -o devtext -lncurses


clean:
	rm devtext
