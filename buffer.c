#include "buffer.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

void lineInit(buffer *bf){
	bf->line = (char *)malloc(sizeof(char) * LINEMAX);
	bf->line = memset(bf->line, '\0', LINEMAX);

}

void bufInit(buffer *bf){
	bf->next = NULL;
	bf->prev = NULL;
	bf->cur_line = 0;
	bf->num_chars = 0;
	if(gettimeofday(&(bf->mod_time), NULL) == -1){
		INFO;
		perror("Time&Date Error: ");
		
	}
	lineInit(bf);

}

void bufCreateNext(buffer *bf){
	bf->next = (buffer *)malloc(sizeof(buffer) * 1);
	bf->next->prev = bf;
	bf->next->next = NULL;
	bf = bf->next;
	bf->cur_line = 0;
	bf->num_chars = 0;
	if(gettimeofday(&(bf->mod_time), NULL) == -1){
		INFO;
		perror("Time&Date Error: ");
	}
	lineInit(bf);

}


void bufDestroy(buffer *bf){
	buffer *temp;
	bf = bf->next;
	while(bf != NULL){
		temp = bf;
		bf = bf->next;
		free(temp);
		free(temp->line);
	}

}

void bufSave(int fd, buffer *bf){
	if(lseek(fd, 0, SEEK_SET) == -1){
		INFO;
		perror("lseek error: ");
	}
	while(bf->next != NULL){
		if(write(fd, bf->line, bf->num_chars) == -1){
			INFO;
			perror("Write error: ");
		}
		bf = bf->next;
	}
}

void bufInsertNext(buffer *bf){
	if(bf->next == NULL){
		bufCreateNext(bf);
		return;
	}
	buffer *temp;
	temp = (buffer *)malloc(sizeof(buffer));
	temp->next = bf->next;
	temp->prev = bf;
	bf->next->prev = temp;
	bf->next = temp;

}

void bufLoad(int fd, buffer *bf){
	char ch;
	int err, linenum = 0, i = 0;
	while((err = read(fd, &ch, 1))){
		if(err == -1){
			INFO;
			perror("Read Error: ");
			exit(0);
		}
		if(i < LINEMAX){

			if(ch == '\n'){
				
				bufCreateNext(bf);
				bf->line[i] = ch;
				bf->cur_line = linenum;
				bf->num_chars = i+1;
				//printf("$$$$: %d\n", bf->cur_line);
				bf = bf->next;
				linenum++;
				//printf("$$$$: %d\n", bf->cur_line);
				i = 0;
				continue;
			}
			else{

				bf->line[i] = ch;
				bf->num_chars = i;

			}

		}
		else{
			
			bufCreateNext(bf);
			bf->line[i] = ch;
			bf->cur_line = linenum;
			bf->num_chars = i+1;
			//printf("####: %d\n", bf->cur_line);
			bf = bf->next;
			linenum++;
			//printf("####: %d\n", bf->cur_line);
			i = 0;
			continue;

		}
	i++;
	}
}




void bufPrintAll(buffer *bf){

	while(bf->next != NULL){
		printf("line no: %d\t\t", bf->cur_line);
		for(int i = 0; i < bf->num_chars; i++)
			printf("%c", *(bf->line + i));
		bf = bf->next;
	}
}

/*int cur_line;
int num_chars;
int curX;
*/

void lineInsert(buffer *bf, int loc, char ch){
	int start, end;
	if(bf->num_chars == 0){
		bf->line[0] = ch;
	}
	else if(bf->num_chars < LINEMAX - 1 && bf->num_chars > 0){

	}
	else if(bf->num_chars == LINEMAX - 1){

	}
}
void lineRemove(buffer *bf, int loc, char ch);







/*void bufSearch(buffer *bf, char *search){
	char *found;
	while(1){
		found = strstr(bf->line, search);
		if(found == NULL && bf->next != NULL){
			bf = bf->next;
		}
	}
	if(found == NULL)
		return NULL;
	else{

		return found;
	}


}*/

/*int main(int argc, char const *argv[])
{
	buffer bf1;
	printf("hi\n");
	int fd = open("test.txt", O_RDWR | O_CREAT , S_IRWXU);
	bufLoad(fd, &bf1);
	bufPrintAll(&bf1);
	bufSave(fd, &bf1);
	close(fd);
	bufDestroy(&bf1);
	printf("\nBUfend\n");
	return 0;
}*/



/*int bufInit(buffer *bf); ///done
void bufDestroy(buffer *bf);//done

void bufSave(int fd, buffer *bf);//done
void bufLoad(int fd, buffer *bf);//done

void bufInsert(buffer *bf);//done
void bufCreateNext(buffer *bf);//done

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
*/