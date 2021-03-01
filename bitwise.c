#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    const char alap[] = "abc";
    *NumCh = (sizeof(alap) / sizeof(char)) - 1;
    char *pixel = (char *)malloc(((*NumCh) * 3) * sizeof(char));
    if (pixel == NULL)
    {
        perror("Memoria foglalas nem sikerult!\n");
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
    char *str = (char *)malloc(NumCh * sizeof(char));
    if (str == NULL)
    {
        perror("Memoria foglalas nem sikerult!\n");
        exit(2);
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
    /*for (int i = 0; i < NumCh * 3; i++)
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
            str[counter] = kenyer;
            counter++;
        }
    }
    */
    free(Pbuff);
    return str;
}
int main()
{
    srand(time(NULL));
    int len;
    char *mem = TestArray(&len);
    char *valasz = Unwrap(mem, len);
    printf("%s\n", valasz);
    free(valasz);
    return 0;
}
