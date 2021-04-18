#include "rkp.h" 

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        int len;
        int f = BrowseForOpen();
        char *valasz = ReadPixels(f, &len);
        signal(SIGALRM, WhatToDo);
        signal(SIGINT, WhatToDo);
        alarm(1);
        char *eredmeny = Unwrap(valasz, len);
        alarm(0);
        puts(eredmeny);
        int post = Post("GN6W3I", eredmeny, len);
        if (post == 0)
        {
            printf("\033[1;32m");
            puts("");
            puts("The text is sent successfully!");
            puts("");
            printf("\033[0m");
        }
        else
        {
            printf("\033[1;31m");
            puts("");
            puts("The text is not delivered!");
            puts("");
            printf("\033[0m");
        }

        free(eredmeny);
    }
    else if (argc == 2 && (strcmp("--version", argv[1])) == 0)
    {
        puts("The creator:   \033[1;34m Zsolt Berecz");
        printf("\033[0m");
        printf("Last update:\t");
        printf("\033[1;37m");
        puts("2021.03.31");
        printf("\033[0m");
        printf("Github link:\t");
        printf("\033[1;36m");
        puts("https://github.com/bzsol/RKP/");
        printf("\033[0m");
        puts("");
    }
    else if (argc == 2 && (strcmp("--help", argv[1]) == 0))
    {
        printf("Functions and Commands:\n");
        printf("----------------------------\n");
        printf("\033[1;32m");
        printf("--version\n");
        printf("\033[0m");
        printf("This command lists the author of the program and the version number/date.\n");
        printf("\033[1;32m");
        printf("./program xxxx.bmp\n");
        printf("\033[0m");
        printf("Directly browse the image file in your browser in an arguement\nyou will be able to decrypt the tutor secret message\nand send it to the internet via HTTP protocol.\n");
        printf("\033[1;32m");
        printf("./program\n");
        printf("\033[0m");
        printf("If you run this program you successfully access the File Browser where you can freely browse for directories or files\n");
        printf("\033[1;31m");
        printf("Signal interruption is blocked!\n");
        printf("\033[0m");
        printf("^C is not working while decoding the image file!\n");
        printf("If you see this message after the program is finished:\n\n");
        printf("\033[1;32m");
        puts("The text is sent successfully!");
        printf("\033[0m");
        printf("The message delivered on time and flawlessly.\n\n");
        printf("\033[1;31m");
        puts("The text is not delivered!");
        printf("\033[0m");
        printf("The message is not delivered on time or the connection is not established normally either the internet connection is not reliable.\n\n");
    }
    else if (argc == 2)
    {

        printf("The opened file: %s\n", argv[1]);
        int len;
        int f = open(argv[1], O_RDONLY);
        char *valasz = ReadPixels(f, &len);
        signal(SIGALRM, WhatToDo);
        signal(SIGINT, WhatToDo);
        alarm(1);
        char *eredmeny = Unwrap(valasz, len);
        alarm(0);
        //puts(eredmeny);
        int post = Post("GN6W3I", eredmeny, len);
        if (post == 0)
        {
            printf("\033[1;32m");
            puts("");
            puts("The text is sent successfully!");
            puts("");
            printf("\033[0m");
        }
        else
        {
            printf("\033[1;31m");
            puts("");
            puts("The text is not delivered!");
            puts("");
            printf("\033[0m");
        }

        free(eredmeny);
    }
    else
    {
        printf("Too many arguements!\n");
        exit(10);
    }
    return 0;
}
