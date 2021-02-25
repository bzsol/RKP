#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* str;
char* pixel;
char* TestArray(int *NumCh){
    const char alap[3] = "abc";
    str = (char*)malloc(256*sizeof(char));
    *NumCh = strlen(alap);
    sprintf(str,"%p",&pixel);
    return str;
}


int main(int argc, char const *argv[])
{

    pixel = (char*)malloc(9*sizeof(char));
    int num;
    char* memcim = TestArray(&num);
    printf("%s\n", memcim);
    free(pixel);
    free(str);

    return 0;
}