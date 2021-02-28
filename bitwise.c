#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void char2bit(char val)
{
    for (int i = 7; 0 <= i; i--)
    {
        printf("%d", (val & (1 << i)) ? 1 : 0);
    }
    printf("\n");
}

char *TestArray(int *NumCh)
{
    char *pixel;
    //char basicmask = 248; // 11111000
    //char firstmask = 252; // 11111100
    char kenyer = 0;
    char zsemle = 3; // 00000011
    char kifli = 56; // 00111000
    char stangi = 7; // 00000111
    char word;
    const char alap[] = "abc";
    *NumCh = sizeof(alap) / sizeof(alap[0]) - 1;
    pixel = (char *)malloc(9 * sizeof(char));
    if (pixel == NULL)
    {
        perror("Hiba! A memoria lefoglalas soran\n");
        exit(1);
    }
    for (int i = 0; i < (*NumCh) * 3; i++)
    {
        kenyer = 0;
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
        if ((i == 0) || (i % 3 == 0))
        {
            for (int j = 5; 0 <= j; j--)
            {
                int x = rand() % (1 - 0 + 1) + 0;
                if (x == 1)
                {
                    kenyer ^= (1 << j);
                }
            }
            kenyer <<= 2;
            word >>= 6;
            word &= zsemle;
            kenyer |= word;
            pixel[i] = kenyer;
        }
        else if ((i == 1) || (i == 4) || (i == 7))
        {
            for (int j = 4; 0 <= j; j--)
            {
                int x = rand() % (1 - 0 + 1) + 0;
                if (x == 1)
                {
                    kenyer ^= (1 << j);
                }
            }
            kenyer <<= 3;
            word &= kifli;
            word >>= 3;
            kenyer |= word;
            pixel[i] = kenyer;
        }
        else
        {
            for (int j = 4; 0 <= j; j--)
            {
                int x = rand() % (1 - 0 + 1) + 0;
                if (x == 1)
                {
                    kenyer ^= (1 << j);
                }
            }
            kenyer <<= 3;
            word &= stangi;
            kenyer |= word;
            pixel[i] = kenyer;
        }
    }
    return pixel;
}
char *Unwrap(char *Pbuff, int NumCh)
{
    char *uzenet = (char *)malloc(NumCh * sizeof(char));
    if (uzenet == NULL)
    {
        perror("Hiba! A memoria lefoglalas soran\n");
        exit(2);
    }
    char kenyer;
    int j = 0;
    for (int i = 0; i < NumCh * 3; i++)
    {
        if ((i == 0) || (i % 3 == 0))
        {
            kenyer = Pbuff[i] << 6;
        }
        else if ((i == 1) || (i == 4) || (i == 7))
        {
            kenyer |= ((Pbuff[i] << 3) & 56);
        }
        else
        {
            kenyer |= (Pbuff[i] & 7);
            uzenet[j] = kenyer;
            j++;
        }
    }
    free(Pbuff);
    return uzenet;
}

int main()
{
    srand(time(NULL));
    int len;
    char *mem = TestArray(&len);
    char *valasz = Unwrap(mem, len);
    puts(valasz);
    free(valasz);
    return 0;
}
