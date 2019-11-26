#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

char* getName(char *line, int index);

int main(int argc, char *argv[]) {
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	FILE *file = fopen(argv[1], "r");
	int nameIndex = -1;
	//char **names = malloc(20000);
	int num = 0;
	
	while((read = getline(&line, &len, file)) != -1) {
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
			printf("%s\n", name);
			// *names = name;
			// name++;
			// num++;
		}
	}

	/*for(int i = 0; i < num; i++) {
		printf("%s\n", (*names-1-i));
	}*/
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