#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "buffer.h"
#include "gui_ncs.h"


/*initialize line*/
void lineInit(buffer *bf){
	bf->line = (char *)malloc(sizeof(char) * LINEMAX);
	bf->line = memset(bf->line, '\0', LINEMAX);

}

/*initialize buffer*/
void bufInit(buffer *bf){
	bf->next = NULL;
	bf->prev = NULL;
	bf->cur_line = 0;
	bf->num_chars = 0;
	lineInit(bf);

}

void bufCreateNext(buffer *bf){ /*when next buffer is null CREATE NEW*/
	bf->next = (buffer *)malloc(sizeof(buffer) * 1);
	bf->next->prev = bf;
	bf->next->next = NULL;
	bf->next->cur_line = bf->cur_line + 1;
	bf = bf->next;
	bf->num_chars = 0;
	lineInit(bf);

}

void bufInsert(buffer *bf){ /*insert b/w bf and bf->next buffers*/
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
	lineInit(bf->next);

}


/*destroys (clears all data in them) all the buffers after and including current buffer. */
void bufDestroy(buffer *bf){
	buffer *temp;

	while(bf != NULL){
		temp = bf;
		bf = bf->next;
		free(temp->line);
		free(temp);
	}

}

/*saves the buffer into the file using file descriptor*/
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


/*loads the data into buffer from text file*/
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



/*this is debugging function, kept for future testing. Prints all the buffer contents on terminal in plain text*/
void bufPrintAll(buffer *bf){

	int i = 0;
	while(bf != NULL){
		printf("line no: %d\t num_chars: %d\t\t@", bf->cur_line, bf->num_chars);
		for(i = 0; i < bf->num_chars; i++){
			if(*(bf->line + i) == '\0'){
				printf("#");
			}	
			else{
				printf("%c", *(bf->line + i));
				
			}	
		}
		printf("@");
		i = 0;
		
		bf = bf->next;
	}
}

void lineInsert(buffer *bf, int loc, char ch){

	if(loc == bf->num_chars){
		bf->line[loc] = ch;
		(bf->num_chars)++;
	}
	else if(bf->num_chars == LINEMAX){
		return;
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
	return;
}

/*Decrese the line number of buffers by value, starting from the same buffer*/
void bufDecr(buffer *bf, int val){
	while(bf->next != NULL){
		bf->cur_line = bf->cur_line - val;
		bf = bf->next;
	}
	(bf->cur_line)--;
}

/*Increase the line number of buffers by value, starting from the same buffer*/
void bufIncr(buffer *bf, int val){
	while(bf->next != NULL){
		bf->cur_line = bf->cur_line + val;
		bf = bf->next;
	}
	(bf->cur_line)++;
}

int fileexist(char const *filename){
     int fd = open(filename, O_RDWR );   /* open file in read/write mode*/
     if(fd == -1)                   /* couldn't open i.e. file not existing*/
        return 0;
     close(fd);   /*close file if opened i.e. file exists*/
     return 1;
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
