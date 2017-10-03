#include "buffer.h"

void lineInit(buffer *bf){
	bf->line = (char *)malloc(sizeof(char) * LINEMAX);
	bf->line = memset(bf->line, 0, LINEMAX);

}

int bufInit(buffer *bf){
	bf->next = NULL;
	bf->prev = NULL;
	bf->cur_line = lines;
	bf->num_chars = 0;
	if(gettimeofday(&(bf->mod_time), NULL) == -1){
		INFO;
		perror("Time&Date Error: ");
		return errno;
	}
	lineInit(bf);
	return TRUE;

}


void bufDestroy(buffer *bf){
	buffer *temp;
	while(bf->next != NULL){
		temp = bf;
		bf=bf->next;
		free(temp->line);
		free(temp);
	}

}

void bufSave(int fd, buffer *bf){
	while(bf->next != NULL){
		if(write(fd, buf->line, bf->num_chars) == -1){
			INFO;
			perror("Write error: ");
		}
		bf = bf->next;
	}
}


void bufLoad(int fd, buffer *bf){

}


int main(int argc, char const *argv[])
{
	printf("hi\n");
	return 0;
}


int bufInit(buffer *bf); ///done
void bufDestroy(buffer *bf);//done

void bufSave(int fd, buffer bf);//done
void bufLoad(int fd, buffer *bf);

void bufCpy(buffer *source, buffer *dest);
void bufSearch(buffer *bf, char *search);
void bufReplace(buffer *bf, char *search, char *replace);

void lineInsert(buffer *bf, int loc, char ch);
void lineRemove(buffer *bf, int loc, char ch);

int lineSearch(char *search);
int lineReplace(char *search, char *replace);

void lineInit(buffer *bf); //done
void linePrint(buffer *bf);
int lineFull(buffer *bf);
int lineEmpty(buffer *bf);
