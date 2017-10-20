#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <sys/time.h>
typedef struct buffer{
	struct buffer *next;
	struct buffer *prev;

	int cur_line;
	int num_chars;
	int ch_mark[10];
	struct timeval mod_time;

	char *line;



}buffer;


#endif