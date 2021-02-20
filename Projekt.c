#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[])
{

	if(argc == 1){

	//This is the buffer and allocated size of the string
	char* filename = malloc(256*sizeof(char));

	// The input
	printf("Please enter the filename: ");
	scanf("%s",filename);

	// We realloc the memory for the better because we don't need 256 char
	filename = realloc(filename,sizeof(filename)/sizeof(filename[0]));
	puts(filename);
    printf("strlen: %d\n",strlen(filename));
    printf("char* size: %d",sizeof(filename)/sizeof(filename[0]));

	// Free the allocated memory
	free(filename);

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
