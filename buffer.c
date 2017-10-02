#include "buffer.h"

void lineInit(buffer *bf){
	bf->line = (char *)malloc(sizeof(char) * LINEMAX);
	bf->line = memset(bf->line, 0, LINEMAX);

}

void bufInit(buffer *bf){
	bf->next = NULL;
	bf->prev = NULL;
	bf->cur_line = lines;
	bf->num_chars = 0;
	if(gettimeofday(bf->mod_time) != 0){
		perror("Time&Date Error: ");
		return errno;
	}
	lineInit(bf);

}









int main(int argc, char const *argv[])
{
	printf("hi\n");
	return 0;
}