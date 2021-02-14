#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	if(argc == 1){
		//char filename[256];
		char* filename = (char*)malloc(256*sizeof(char));
		puts("Please enter a filename");
		scanf("%s",filename);
		printf("You work on this file: ");
		puts(filename);
		char* file = malloc(strlen(filename) + 1);
		strcpy(file,filename);
		printf("This is the char* file: %s\n",file);
		free(filename); // For the overflow
		
	}
	else if(argc == 2 && (strcmp("--version",argv[1])) == 0){
		printf("########################\n");
		printf("#  Berecz Zsolt-GN6W3I #\n");
		printf("########################\n");
		printf("########################\n");
		printf("#     Version: 1.0.0   #\n");
		printf("########################\n");
		printf("########################\n");
		printf("#    Date: 11.02.2021  #\n");
		printf("########################\n");
		
	}
	else if(argc == 2 && (strcmp("--help",argv[1]) == 0)){
		printf("Functions and Commands:\n");
		printf("----------------------------\n");
		printf("--version\n -> This command lists the author of the program and the version number/date\n");
	}
	else if(argc == 2){
		printf("%s\n",argv[1]);
	}
	return 0;
}
