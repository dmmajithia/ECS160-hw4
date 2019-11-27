#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

struct tweeter {
	char *name;
	int count;
};

char* getName(char *line, int index);
void getNames(char *filename, int *num, char *names[]);
void insert(char* name, struct tweeter *tweeters, int *len);
void updateOrder(int current, struct tweeter *tweeters);

int main(int argc, char *argv[]) {
	char *filename = argv[1];
	int num, len = 0; // num = number of lines in file, len = number of unique usernames
	struct tweeter tweeters[20000];
	char *names[20000];
	getNames(filename, &num, names);
	for(int i = 0; i < num; i++) {
		insert(*(names+i), tweeters, &len);
	}
	if(len > 10) {
		len = 10;
	}
	for(int i = 0; i < len; i++) {
		printf("%s: %d\n", (tweeters+i)->name, (tweeters+i)->count);
	}
}

void insert(char* name, struct tweeter *tweeters, int *len) {
	for(int i = 0; i < *len; i++) {
		if(strcmp(name, (tweeters+i)->name) == 0) { // found name
			(tweeters+i)->count = (tweeters+i)->count + 1;
			updateOrder(i, tweeters);
			return; //return after updating everything
		}
	}
	//if we made it this far, the name does not exist in the array
	struct tweeter t = {name, 1};
	*(tweeters+ *len) = t;
	(*len)++;
}

void updateOrder(int current, struct tweeter *tweeters) {
	if(current > 0) {
		if ((tweeters+current-1)->count < (tweeters+current)->count) { //then swap them, and recurse
			struct tweeter temp = *(tweeters+current-1);
			*(tweeters+current-1) = *(tweeters+current);
			*(tweeters+current) = temp;
			updateOrder(current-1, tweeters);
		}
	}
}

void getNames(char *filename, int *num, char *names[]) {
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	FILE *file = fopen(filename, "r");
	int nameIndex = -1;
	
	while((read = getline(&line, &len, file)) != -1) {
		if(len == 0){
			continue;
		}
		if(nameIndex < 0) {
			char *p = strtok(line, ",");
			while(p != NULL) {
				nameIndex++;
				if(strcmp(p, "name") == 0) {
					break;
				}
				p = strtok(NULL, ",");
			}
		}
		else{
			char *name = getName(line, nameIndex);
			*(names + *num) = malloc(strlen(name)+1);
			strcpy(*(names + *num), name);
			(*num)++;
		}
	}
}

char* getName(char *line, int index) {
	int currentIndex = 0; //index of commas passed on the line
	while(currentIndex < index) { //advance line pointer to the tweeter
		if(*line == ','){
			currentIndex++;
		}
		line++;
	}
	char *name = strtok(line, ",");
	return name;
}