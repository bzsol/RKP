#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void char2bit(char val)
{
    for (int i = 7; 0 <= i; i--)
    {
        printf("%d", (val & (1 << i)) ? 1 : 0);
    }
    printf("\n");
}
void long2bit(unsigned long val)
{
    for (int i = 31; 0 <= i; i--)
    {
        printf("%d", (val & (1 << i)) ? 1 : 0);
    }
    printf("\n");
}

char *Unwrap(char *Pbuff, int NumCh)
{
    char *fin = (char *)malloc(NumCh * sizeof(char));
    char *str = (char *)malloc(NumCh * sizeof(char));
    if (fin == NULL)
    {
        perror("The memory allocation failed!");
        exit(1);
    }
    else if (str == NULL)
    {
        perror("The memory allocation failed!");
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
    free(Pbuff);
    strcpy(fin, str);
    free(str);
    return fin;
}

char *ReadPixels(int f, int *NumCh)
{
    unsigned long kenyer = 0;
    unsigned long zsemle = 0;
    char *p = (char *)malloc(4 * sizeof(char));
    char *q = (char *)malloc(4 * sizeof(char));
    char *bm = (char *)calloc(2,sizeof(char));
    read(f, bm, 2);
    if (strcmp(bm, "BM") != 0)
    {
        perror("This is not a valid BMP file!");
        free(bm);
        exit(3);
    }
    else if(bm == NULL){
        perror("The memory allocation failed!");
        exit(1);
    }
    else if(p == NULL){
        perror("The memory allocation failed!");
        exit(1);
    }
    else if(q == NULL){
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
    if(pixel == NULL){
        perror("The memory allocation failed!");
        exit(1);
    }
    lseek(f, zsemle, SEEK_SET);
    read(f, pixel, kenyer * 3);
    *NumCh = (int)kenyer;
    free(q);
    free(p);
    return pixel;
}
int main(int argc, char const *argv[])
{
    int len;
    int f = open("cpu.bmp", O_RDONLY);
    char *valasz = ReadPixels(f, &len);
    char *eredmeny = Unwrap(valasz, len);
    printf("%s\n",eredmeny);
    free(eredmeny);
}
