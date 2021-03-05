#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void char2bit(char val)
{
	for (int i = 7; 0 <= i; i--)
	{
		printf("%d", (val & (1 << i)) ? 1 : 0);
	}
	printf("\n");
}
void RandomPixel(char *random)
{
	for (int k = 0; k <= 7; k++)
	{
		int x = rand() % 2;
		if (x == 1)
		{
			*random ^= (1 << k);
		}
	}
}
void long2bit(unsigned long val)
{
	for (int i = 31; 0 <= i; i--)
	{
		printf("%d", (val & (1 << i)) ? 1 : 0);
	}
	printf("\n");
}
char *ReadPixels(int f, int *NumCh)
{
	unsigned long kenyer = 0;
	unsigned long zsemle = 0;
	char *p = (char *)malloc(4 * sizeof(char));
	char *q = (char *)malloc(4 * sizeof(char));
	char *bm = (char *)malloc(2 * sizeof(char));
	read(f, bm, 2);
	if (strcmp(bm, "BM") != 0)
	{
		perror("This is not a valid BMP file!");
		free(bm);
		exit(3);
	}
	else if (bm == NULL)
	{
		perror("The memory allocation failed!");
		exit(1);
	}
	else if (p == NULL)
	{
		perror("The memory allocation failed!");
		exit(1);
	}
	else if (q == NULL)
	{
		perror("The memory allocation failed!");
		exit(1);
	}
	free(bm);
	lseek(f, 6, SEEK_SET); // 6. byte-tól megyünk
	read(f, p, 4);
	lseek(f, 10, SEEK_SET); // 10. byte-tól
	read(f, q, 4);
	char array[4];
	char offset[4];
	for (int i = 3; i >= 0; i--)
	{
		array[i] = p[3 - i];
		offset[i] = q[3 - i];
	}
	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			zsemle |= offset[i];
			zsemle <<= 24;
			kenyer |= array[i];
			kenyer <<= 24;
		}
		if (i == 1)
		{
			zsemle |= (offset[i] << 16) & 16711680;
			kenyer |= (array[i] << 16) & 16711680;
		}
		else if (i == 2)
		{
			zsemle |= (offset[i] << 8) & 65280;
			kenyer |= (array[i] << 8) & 65280;
		}
		else if (i == 3)
		{
			zsemle |= (offset[i] & 255);
			kenyer |= (array[i] & 255);
		}
	}
	char *pixel = (char *)malloc((kenyer * 3) * sizeof(char));
	if (pixel == NULL)
	{
		perror("The memory allocation failed!");
		exit(1);
	}
	lseek(f, zsemle, SEEK_SET);
	read(f, pixel, kenyer * 3);
	*NumCh = kenyer;
	free(q);
	free(p);
	return pixel;
}
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
				RandomPixel(&random);
				random <<= 2;
				kenyer = ((alap[i] >> 6) & 3) | random;
				pixel[counter] = kenyer;
				counter++;
			}
			else if (j == 1)
			{
				RandomPixel(&random);
				random <<= 3;
				kenyer = ((alap[i] >> 3) & 7) | random;
				pixel[counter] = kenyer;
				counter++;
			}
			else if (j == 2)
			{
				RandomPixel(&random);
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

		// Free the memory for the better because we don't need 256 char
		char *budget = (char *)malloc(strlen(filename) * sizeof(char));
		strcpy(budget, filename);
		free(filename);
		printf("The opened file: %s\n", budget);
		int len;
		int f = open(budget, O_RDONLY);
		char *valasz = ReadPixels(f, &len);
		char *eredmeny = Unwrap(valasz, len);
		printf("%s\n", eredmeny);
		free(eredmeny);
		// Free the allocated memory
		free(budget);
	}
	else if (argc == 2 && (strcmp("--version", argv[1])) == 0)
	{
		printf("########################\n");
		printf("#  Berecz Zsolt-GN6W3I #\n");
		printf("########################\n");
		printf("########################\n");
		printf("#     Version: 1.1     #\n");
		printf("########################\n");
		printf("########################\n");
		printf("#    Date: 2.03.2021   #\n");
		printf("########################\n");
	}
	else if (argc == 2 && (strcmp("--help", argv[1]) == 0))
	{
		printf("Functions and Commands:\n");
		printf("----------------------------\n");
		printf("--version\nThis command lists the author of the program and the version number/date\n");
		printf("--testarray\n This command tests to write a string to bmp pixels and after that decodes that\n");
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
		// Can use the argv[1] to the name of the file
		printf("The opened file: %s\n", argv[1]);
		int len;
		int f = open(argv[1], O_RDONLY);
		char *valasz = ReadPixels(f, &len);
		char *eredmeny = Unwrap(valasz, len);
		printf("%s\n", eredmeny);
		free(eredmeny);
	}
	else
	{
		printf("Too many arguements!\nTry again!\n");
	}
	return 0;
}
