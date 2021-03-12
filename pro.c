#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#define GRN "\x1B[32m"
#define RED "\x1B[31m"
#define RST "\x1B[0m"
#define BLU "\x1B[34m"

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

char *TestArray(int *NumCh)
{
    const char alap[] = "megszentsegtelenithetetlensegeskedeseitekert";
    *NumCh = (sizeof(alap) / sizeof(char)) - 1;
    char *pixel = (char *)malloc(((*NumCh) * 3) * sizeof(char));
    if (pixel == NULL)
    {
        perror("The memory allocation is failed!\n");
        exit(1);
    }
    int counter = 0;
    for (int i = 0; i < *NumCh; i++)
    { // karaktereken
        for (int j = 0; j < 3; j++)
        { //pixeleken
            char random;
            char kenyer;
            if (j == 0)
            {
                RandomPixel(&random);
                random <<= 2;
                kenyer = ((alap[i] >> 6) & 3) | random;
                pixel[counter] = kenyer;
            }
            else if (j == 1)
            {
                RandomPixel(&random);
                random <<= 3;
                kenyer = ((alap[i] >> 3) & 7) | random;
                pixel[counter] = kenyer;
            }
            else if (j == 2)
            {
                RandomPixel(&random);
                random <<= 3;
                kenyer = (alap[i] & 7) | random;
                pixel[counter] = kenyer;
            }
            counter++;
        }
    }
    return pixel;
}
char *Unwrap(char *Pbuff, int NumCh)
{
    char *str = (char *)calloc(NumCh, sizeof(char));
    if (str == NULL)
    {
        perror("The memory allocation is failed!\n");
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
            }
            else if (j == 1)
            {
                kenyer |= ((Pbuff[counter] << 3) & 0x38);
            }
            else if (j == 2)
            {
                kenyer |= (Pbuff[counter] & 7);
            }
            counter++;
        }
        str[i] = kenyer;
    }
    // This will work in windows operating system too!
    free(Pbuff);
    return str;
}
char *ReadPixels(int f, int *NumCh)
{
    char *bm = (char *)calloc(2, sizeof(char));
    unsigned int len;
    unsigned int zsemle;
    read(f, bm, 2);
    if (strcmp(bm, "BM") != 0)
    {
        perror("This is not a valid BMP file!");
        free(bm);
        exit(3);
    }
    else if (bm == NULL)
    {
        perror("The memory allocation is failed!\n");
        exit(1);
    }
    free(bm);
    lseek(f, 6, SEEK_SET); // 6. byte-tól megyünk
    read(f, &len, 4);
    lseek(f, 10, SEEK_SET); // 10. byte-tól
    read(f, &zsemle, 4);
    char *pixel = (char *)malloc((len * 3) * sizeof(char));
    if (pixel == NULL)
    {
        perror("The memory allocation is failed!");
        exit(1);
    }
    lseek(f, zsemle, SEEK_SET);
    read(f, pixel, len * 3);
    *NumCh = len;
    close(f);
    return pixel;
}
int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        puts("File browser!");
    }
    else if (argc == 2 && (strcmp("--version", argv[1])) == 0)
    {
        printf(RED "▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇\n" RST);
        printf("▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇\n");
        printf(GRN "▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇\n\n\n" RST);
        printf("1000%% magyar");
        printf(RED " <3 \n" RST);
        puts("The creator is Zsolt Berecz");
        puts("2021.03.12");
    }
    else if (argc == 2 && (strcmp("--help", argv[1]) == 0))
    {
        printf("Functions and Commands:\n");
        printf("----------------------------\n");
        printf(GRN "--version\n" RST);
        printf("This command lists the author of the program and the version number/date. (Hungarian flag included because why not)\n");
        printf(GRN "--testarray\n" RST);
        printf("This command show one of the marvelous Hungarian words you can find in your grammar book.\n");
        printf(GRN "./program xxxx.bmp\n" RST);
        printf("You will be able to decrypt the professor secret message.\n");
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
