#include "buffer.h"
#include "gui_ncs.h"


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

void bufCreateNext(buffer *bf){ //when next buffer is null CREATE NEW
	bf->next = (buffer *)malloc(sizeof(buffer) * 1);
	bf->next->prev = bf;
	bf->next->next = NULL;
	bf->next->cur_line = bf->cur_line + 1;
	bf = bf->next;
	bf->num_chars = 0;
	if(gettimeofday(&(bf->mod_time), NULL) == -1){
		INFO;
		perror("Time&Date Error: ");
	}
	lineInit(bf);

}

void bufInsert(buffer *bf){ //insert b/w bf and bf->next buffers
	if(bf->next == NULL){
		bufCreateNext(bf);
		return;
	}
	bufIncr(bf->next, 1);
	buffer *temp;
	temp = (buffer *)malloc(sizeof(buffer));
	temp->next = bf->next;
	temp->prev = bf;
	bf->next->prev = temp;
	bf->next = temp;
	bf->next->cur_line = bf->cur_line + 1;
	bf->next->num_chars = 0;
	if(gettimeofday(&(bf->next->mod_time), NULL) == -1){
		INFO;
		perror("Time&Date Error: ");
	}
	lineInit(bf->next);

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
	while(bf != NULL){
		if(write(fd, bf->line, bf->num_chars) == -1){
			INFO;
			perror("Write error: ");
		}
		bf = bf->next;
	}
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
				bf = bf->next;
				linenum++;
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
			bf = bf->next;
			linenum++;
			i = 0;
			continue;

		}
	i++;
	}
	bf = bf->prev;
	free(bf->next);
	bf->next = NULL;
}




void bufPrintAll(buffer *bf){

	while(bf != NULL){
		printf("line no: %d\t num_chars: %d\t\t@", bf->cur_line, bf->num_chars);
		for(int i = 0; i < bf->num_chars; i++){
			if(*(bf->line + i) == '\0'){
				printf("#");
			}	
			else{
				printf("%c", *(bf->line + i));
				
			}	
		}
		printf("@");
		
		bf = bf->next;
	}
}

void lineInsert(buffer *bf, int loc, char ch){

	if(loc == bf->num_chars){
		bf->line[loc] = ch;
		(bf->num_chars)++;
	}
	else if(bf->num_chars == 0){
		bf->line[0] = ch;
		(bf->num_chars)++;
	}
	else if(bf->num_chars < LINEMAX && bf->num_chars > 0 && loc != LINEMAX && loc != LINEMAX - 1){
		memmove((bf->line + loc + 1), (bf->line + loc), bf->num_chars - loc);
		bf->line[loc] = ch;
		(bf->num_chars)++;
	}
	else if(bf->num_chars == LINEMAX){
		return;
	}
}

void bufDecr(buffer *bf, int val){
	while(bf->next != NULL){
		(bf->cur_line)--;
		bf = bf->next;
	}
	(bf->cur_line)--;
}

void bufIncr(buffer *bf, int val){
	while(bf->next != NULL){
		(bf->cur_line)++;
		bf = bf->next;
	}
	(bf->cur_line)++;
}

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