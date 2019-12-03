#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

struct tweeter {
	char *name;
	int count;
};

char* getName(char *line, int index, int fields);
void getNames(char *filename, int *num, char *names[]);
void insert(char* name, struct tweeter *tweeters, int *len);
void updateOrder(int current, struct tweeter *tweeters);
void invalidInputFormat();
void removeChar(char *s, int c);

int main(int argc, char *argv[]) {
	if(argc != 2) {
		invalidInputFormat();
	}
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
	for(int i = 0; i < *len; i++) { // first look for given name in the tweeters array
		if(strcmp(name, (tweeters+i)->name) == 0) { // found name
			(tweeters+i)->count = (tweeters+i)->count + 1; //update count for that name
			updateOrder(i, tweeters);
			return; //return after updating everything
		}
	}
	//if we made it this far, the name does not exist in the array
	struct tweeter t = {name, 1};
	*(tweeters+ *len) = t;
	(*len)++;
}

void updateOrder(int current, struct tweeter *tweeters) { // after every insertion, make sure array remains in descending order of counts
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
	int fields = 0; // number of fields in header line
	bool quotedCol = false;

	if(file == NULL) {
		invalidInputFormat();
	}
	
	while((read = getline(&line, &len, file)) != -1) {
		if(*num >= 20000-1) {
			invalidInputFormat();
		}
		if(read > 1024) {
			invalidInputFormat();
		}
		if(len == 0){
			continue;
		}
		if(nameIndex < 0) {
			bool found = false;
			char *p = strtok(line, ",");
			while(p != NULL) {
				fields++;
				if(!found) {
					nameIndex++;
				}
				if(strcmp(p, "name") == 0 || strcmp(p, "\"name\"") == 0) {
					if(strcmp(p, "\"name\"") == 0) {
						quotedCol = true;
					}
					if(found) { // if multiple name fields found, invalid.
						invalidInputFormat();
					}
					else {
						found = true;
					}
				}
				p = strtok(NULL, ",");
			}
			if(!found) { // if we made it past the while loop without finding a name field, report invalid input format :/
				invalidInputFormat();
			}
		}
		else{
			char *name = getName(line, nameIndex, fields);
			if(name == NULL) {
				continue;
			}
			if(quotedCol) {
				//check if name has quotes
				if(*name == '\"' && *(name+strlen(name)-1) == '\"') {
					removeChar(name, '\"');
				}
				else{
					invalidInputFormat();
				}
			}
			*(names + *num) = malloc(strlen(name)+1);
			strcpy(*(names + *num), name);
			(*num)++;
		}
	}
}

char* getName(char *line, int index, int fields) {
	removeChar(line, '\n');
	removeChar(line, ' ');
	if(strlen(line) == 1) {
		return NULL;
	}
	char *lineCopy = (char*) malloc(strlen(line)+1);
	strcpy(lineCopy, line);
	int currentIndex = 0; //index of commas passed on the line
	while(currentIndex < index) { //advance line pointer to the tweeter
		if(*line == ','){
			currentIndex++;
		}
		line++;
		lineCopy++;
	}
	char *name = strtok(line, ",");
	//now we have to check if the number of fields on this line is the same as the header line
	// AKA check for commas inside tweet
	while(strlen(lineCopy) > 0) {
		if(*lineCopy == ',') {
			currentIndex++;
		}
		lineCopy++;
	}
	if(*lineCopy != ',') {
		currentIndex++;
	}
	if(currentIndex != fields) {
		//printf("currentIndex = %d, fields = %d\n", currentIndex, fields);
		invalidInputFormat();
	}
	return name;
}

void removeChar(char *s, int c){ 
    int j, n = strlen(s); 
    for (int i=j=0; i<n; i++) 
       if (s[i] != c) 
          s[j++] = s[i]; 
      
    s[j] = '\0'; 
}

void invalidInputFormat() {
	// Called when - 
	// 1. More than one command line argument present ✅
	// 2. No "name" index is/ mumltiple name indices found in header of csv file ✅
	// 3. File not found ✅
	// 4. Line length exceeds 1024 bytes ✅
	// 5. File length exceeds 20,000 lines ✅
	// 6 Commas inside tweet ✅
	// 
	printf("Invalid Input Format.\n");
	exit(0);
}