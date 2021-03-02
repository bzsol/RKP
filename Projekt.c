#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *TestArray(int *NumCh)
{
	const char alap[] = "megszentsegtelenithetetlensegeskedeseitekert";
	*NumCh = (sizeof(alap) / sizeof(char)) - 1;
	char *pixel = (char *)malloc(((*NumCh) * 3) * sizeof(char));
	if (pixel == NULL)
	{
		perror("Hiba! A memoria lefoglalasakor\n");
		exit(1);
	}
	int counter = 0;
	for (int i = 0; i < *NumCh; i++)
	{ // karaktereken
		for (int j = 0; j < 3; j++)
		{ // pixeleken
			char random;
			char kenyer; // pixel[]?!
			if (j == 0)
			{
				for (int k = 0; k <= 5; k++)
				{
					int x = rand() % 2;
					if (x == 1)
					{
						random ^= (1 << k);
					}
				}
				random <<= 2;
				kenyer = ((alap[i] >> 6) & 3) | random;
				pixel[counter] = kenyer;
				counter++;
			}
			else if (j == 1)
			{
				for (int k = 0; k <= 4; k++)
				{
					int x = rand() % 2;
					if (x == 1)
					{
						random ^= (1 << k);
					}
				}
				random <<= 3;
				kenyer = ((alap[i] >> 3) & 7) | random;
				pixel[counter] = kenyer;
				counter++;
			}
			else if (j == 2)
			{
				for (int k = 0; k <= 4; k++)
				{
					int x = rand() % 2;
					if (x == 1)
					{
						random ^= (1 << k);
					}
				}
				random <<= 3;
				kenyer = (alap[i] & 7) | random;
				pixel[counter] = kenyer;
				counter++;
			}
		}
	}
	return pixel;
}
char *Unwrap(char *Pbuff, int NumCh)
{
	char *fin = (char *)malloc(NumCh * sizeof(char));
	char *str = (char *)malloc(NumCh * sizeof(char));
	if (fin == NULL)
	{
		perror("Hiba! A memoria lefoglalasakor\n");
		exit(1);
	}
	else if (str == NULL)
	{
		perror("Hiba! A memoria lefoglalasakor\n");
		exit(1);
	}
	int counter = 0;
	for (int i = 0; i < NumCh; i++)
	{
		char kenyer;
		for (int j = 0; j < 3; j++)
		{
			if (j == 0)
			{
				kenyer = Pbuff[counter] << 6;
				counter++;
			}
			else if (j == 1)
			{
				kenyer |= ((Pbuff[counter] << 3) & 56);
				counter++;
			}
			else if (j == 2)
			{
				kenyer |= (Pbuff[counter] & 7);
				counter++;
			}
		}

		str[i] = kenyer;
	}
	// This will work in windows operating system too!
	free(Pbuff);
	strcpy(fin, str);
	free(str);
	return fin;
}

int main(int argc, char const *argv[])
{

	if (argc == 1)
	{

		//This is the buffer and allocated size of the string
		char *filename = malloc(256 * sizeof(char));

		// The input
		printf("Please enter the filename: ");
		scanf("%s", filename);

		// We realloc the memory for the better because we don't need 256 char
		filename = realloc(filename, sizeof(filename) / sizeof(filename[0]));
		puts(filename);
		printf("strlen: %d\n", strlen(filename));
		printf("char* size: %d", sizeof(filename) / sizeof(filename[0]));

		// Free the allocated memory
		free(filename);
	}
	else if (argc == 2 && (strcmp("--version", argv[1])) == 0)
	{
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
	else if (argc == 2 && (strcmp("--help", argv[1]) == 0))
	{
		printf("Functions and Commands:\n");
		printf("----------------------------\n\n");
		printf("--version\nThis command lists the author of the program and the version number/date\n");
	}
	else if (argc == 2 && (strcmp("--testarray", argv[1]) == 0))
	{
		srand(time(NULL));
		int len;
		char *mem = TestArray(&len);
		char *valasz = Unwrap(mem, len);
		printf("%s\n", valasz);
		free(valasz);
	}
	else if (argc == 2)
	{
		// We can use the argv[1] to the name of the file
		printf("%s\n", argv[1]);
	}
	return 0;
}
