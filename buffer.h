#define LINEMAX 64



typedef struct buffer{
	struct buffer *next;
	struct buffer *prev;

	int  cur_line;
	int num_chars;
	int ch_mark[10];
	int buff_no;
	timeval mod_time;

	char line[LINEMAX];



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

void linePrint(buffer *bf);
int lineFull(buffer *bf);
int lineEmpty(buffer *bf);

