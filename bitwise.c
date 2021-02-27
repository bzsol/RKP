#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char pixel;
char* str;
char* TestArray(int *NumCh){
    char changer;
    const char alap[3] = "abc";
    *NumCh = sizeof(alap)/sizeof(alap[0]);

    // 4 alapeset
    for(int i = 5;0<=i;i--){
        int x = rand()%(1-0+1)+0;
            //printf("%d",x);
            if(x == 1){
                pixel ^= (1 << i);
            }
    }
    pixel <<= 2; // 3 alapeset

    changer = (changer|alap[0]);
    changer >>= 6;
    pixel = (pixel|changer);
    str = (char*)malloc(100*sizeof(char));
    sprintf(str,"%p",&pixel);
 return str;
}



void charbit(char val) {
  for (int i=7; 0<=i;i--) {
    printf("%c", (val & (1 << i)) ? '1' : '0');
  }
  printf("\n");
}


int main(int argc, char const *argv[])
{
    int num;
    //charbit(pixel);
    char* alap = TestArray(&num);
    printf("pixel: ");
    charbit(pixel);
    printf("\n");
    printf("titkos hossz: %d\n",num);
    printf("memcim: %s\n",str);
    free(str);
    return 0;
}
