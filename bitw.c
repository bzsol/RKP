#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *pixel;
char *str;
char word;
char *TestArray(int *NumCh)
{
    // The secret word
    const char alap[3] = "abc";
    // Return the number of words
    *NumCh = sizeof(alap) / sizeof(alap[0]);

    pixel = (char *)malloc(9 * sizeof(char));
    for (int i = 0; i < 9; i++)
    {
        char changer = 0x0000;
        char buff;
        if (i < 3)
        {
            word = alap[0];
        }
        else if (i < 6)
        {
            word = alap[1];
        }
        else
        {
            word = alap[2];
        }
        if ((i == 0) || i % 3 == 0)
        {

            for (int j = 5; 0 <= j; j--)
            {
                int x = rand() % (1 - 0 + 1) + 0;
                //printf("%d",x);
                if (x == 1)
                {
                    buff ^= (1 << j);
                }
            }
            buff <<= 2;
            changer |= word;
            changer >>= 6;
            buff |= changer;
            pixel[i] = buff;
        }
        else if ((i == 1) || (i == 4) || (i == 7))
        {
            for (int j = 4; 0 <= j; j--)
            {
                int x = rand() % (1 - 0 + 1) + 0;
                //printf("%d",x);
                if (x == 1)
                {
                    buff ^= (1 << j);
                }
            }
            buff <<= 3;
            changer |= word;
            changer <<= 2;
            changer >>= 5;
            buff |= changer;
            pixel[i] = buff;
        }
        else
        {
            for (int j = 4; 0 <= j; j--)
            {
                int x = rand() % (1 - 0 + 1) + 0;
                //printf("%d",x);
                if (x == 1)
                {
                    buff ^= (1 << j);
                }
            }
            buff <<= 3;
            changer |= word;
            changer <<= 5;
            changer >>= 5;
            buff |= changer;
            pixel[i] = buff;
        }
    }

    // Return the mem address to the function
    str = (char *)malloc(30 * sizeof(char));
    sprintf(str, "%p", &pixel);

    return str;
}

void char2bit(char val)
{
    for (int i = 7; 0 <= i; i--)
    {
        printf("%c", (val & (1 << i)) ? '1' : '0');
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    int num;
    char *tarolo = TestArray(&num);
    char2bit(pixel[0]);
    char2bit(pixel[1]);
    char2bit(pixel[2]);

    printf("titkos len: %d\n", num);
    printf("mem cim: %s\n", str);
    free(str);
    free(pixel);
    return 0;
}
