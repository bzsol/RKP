#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[])
{

	if(argc == 1){
		
		// The user enter the filename so we can store in the memory.
		//char filename[256];
		char* filename = malloc(256*sizeof(char));
		printf("Enter the file: ");
		scanf("%s",filename);
		int counter=0;
		
		// Count how much char we need to store exactly
		while(filename[counter] != '\0'){
			counter++;
		}
		
		// The budget know how much space need to store the exact char* so we care about how much memory we use
		// At the moment
		char* budget = malloc(counter*sizeof(char));
		strcpy(budget,filename);
		free(filename);
		printf("%s\n%ld\n",budget,strlen(budget));

		// Free the budget because don't really use for anything at the moment
		free(budget);
		
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
		printf("----------------------------\n\n");
		printf("--version\nThis command lists the author of the program and the version number/date\n");
	}
	else if(argc == 2){
		// We can use the argv[1] to the name of the file
		printf("%s\n",argv[1]);
	}
	return 0;
}
