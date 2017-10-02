#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

#define LINEMAX 64
int lines = 0;


typedef struct buffer{
	struct buffer *next;
	struct buffer *prev;

	int  cur_line;
	int num_chars;
	int ch_mark[10];
	struct timeval mod_time;

	char *line;



}buffer;

void bufInit(buffer *bf);
void bufDestroy(buffer *bf);

void bufSave(int fd, buffer bf);
void bufLoad(int fd, buffer *bf);

void bufCpy(buffer *source, buffer *dest);
void bufSearch(buffer *bf, char *search);
void bufReplace(buffer *bf, char *search, char *replace);

void lineInsert(buffer *bf, int loc, char ch);
void lineRemove(buffer *bf, int loc, char ch);

int lineSearch(char *search);
int lineReplace(char *search, char *replace);

void lineInit(buffer *bf);
void linePrint(buffer *bf);
int lineFull(buffer *bf);
int lineEmpty(buffer *bf);
