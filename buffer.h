#ifndef BUFFER_H
#define BUFFER_H 


/*#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>*/
#include <sys/time.h>

#define INFO printf("\nline %d : %s : %s\n", __LINE__, __FILE__, __func__)
#define LINEMAX 80
#define TRUE 1
#define FALSE 0
//int lines = 0;


typedef struct buffer{
	struct buffer *next;
	struct buffer *prev;

	int cur_line;
	int num_chars;
	int curX;

	int ch_mark[10];
	struct timeval mod_time;

	char *line;



}buffer;

void bufInit(buffer *bf); ///done
void bufDestroy(buffer *bf);

void bufSave(int fd, buffer *bf);
void bufLoad(int fd, buffer *bf);

void bufCreateNext(buffer *bf);
void bufInsert(buffer *bf);

void bufCpy(buffer *source, buffer *dest);
void bufSearch(buffer *bf, char *search);
void bufReplace(buffer *bf, char *search, char *replace);

void bufPrintAll(buffer *bf);

void lineInsert(buffer *bf, int loc, char ch);
void lineRemove(buffer *bf, int loc, char ch);

int lineSearch(char *search);
int lineReplace(char *search, char *replace);

void lineInit(buffer *bf); //done
void linePrint(buffer *bf);
int lineFull(buffer *bf);
int lineEmpty(buffer *bf);

void bufIncr(buffer *bf, int val);
void bufDecr(buffer *bf, int val);

#endif