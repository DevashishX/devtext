#ifndef BUFFER_H
#define BUFFER_H 


#define INFO printf("\nline %d : %s : %s\n", __LINE__, __FILE__, __func__)
#define LINEMAX 180

#define TRUE 1
#define FALSE 0



typedef struct buffer{
	struct buffer *next;
	struct buffer *prev;

	int cur_line;
	int num_chars;
	int curX;
	char *line;



}buffer;

void bufInit(buffer *bf);
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
buffer* lineRemove(buffer *bf, int y, int x);

int lineSearch(char *search);
int lineReplace(char *search, char *replace);

void lineInit(buffer *bf);
void linePrint(buffer *bf);
int lineFull(buffer *bf);
int lineEmpty(buffer *bf);

void bufIncr(buffer *bf, int val);
void bufDecr(buffer *bf, int val);

int fileexist(char const *filename);

#endif
