plan.h

typedef struct buffer{
	struct buffer *next;
	char buff_name[BUFFNAMEMAX];
	int cur_line;
	int num_lines;
	int num_char;
	storage line;

}


typedef struct storage{
	char line[512];
}storage;

