#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define TCP 80
#define BUFFSIZE 1024

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
    unsigned int len;    // Mekkora hossz
    unsigned int zsemle; // Honnan
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
    char *pixel = (char *)malloc((len * 3) * sizeof(char)); // R G B
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
int BrowseForOpen()
{
    int f;
    DIR *dir;
    struct dirent *dp;
    chdir(getenv("HOME"));
    struct stat inode;
    char *filename = (char *)malloc(FILENAME_MAX * sizeof(char));
    char *path = (char *)malloc(PATH_MAX * sizeof(char));
    char *input = (char *)malloc(FILENAME_MAX * sizeof(char));
    while (1)
    {
        dir = opendir(".");
        while ((dp = readdir(dir)) != NULL)
        {
            strcpy(filename, dp->d_name);
            if (dp->d_type == DT_DIR)
            {
                printf("\033[1;34m");
                printf("Directory: ");
                printf("\033[0m");
            }
            else if (dp->d_type == DT_REG)
            {
                printf("\033[1;32m");
                printf("File: ");
                printf("\033[0m");
            }
            printf(" %s \n", filename);
        }
        closedir(dir);
        printf("The current directory: \033[1;36m %s\n", getcwd(path, FILENAME_MAX));
        printf("\033[0m");
        do
        {
            printf(">> ");
            scanf("%s", input);
        } while (stat(input, &inode) != 0);
        printf("\n");
        if (inode.st_mode & S_IFDIR)
        {
            chdir(input);
        }
        else if (inode.st_mode & S_IFREG)
        {
            f = open(input, O_RDONLY);
            break;
        }
        system("clear");
    }
    free(filename);
    free(path);
    free(input);
    system("clear");
    return f;
}

int Post(char *neptunID, char *message, int NumCh)
{
    int sock;
    int con;
    int sent;
    int rec;
    char IP[] = "193.6.135.162";
    struct sockaddr_in address;
    struct hostent *hostinfo;
    char *buffer = malloc(BUFFSIZE * sizeof(char)); // 1kB adat
    char *received;
    hostinfo = gethostbyname(IP);
    if (hostinfo == NULL)
    {
        perror("Error! The host is not found!");
    }
    sprintf(buffer, "POST /~vargai/post.php HTTP/1.1\r\nHost: irh.inf.unideb.hu\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nNeptunID=%s&PostedText=%s", NumCh + 6 + 9 + 12, neptunID, message);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Error! Opening the socket is failed!");
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(TCP); // define 80 port 
    address.sin_addr.s_addr = inet_addr(IP);
    con = connect(sock, (struct sockaddr *)&address, sizeof(address));
    if (con < 0)
    {
        perror("Connection with the server is not established!");
    }
    //write(sock, buffer, strlen(buffer));
    sent = send(sock, buffer, strlen(buffer), 0);
    if (sent <= 0)
    {
        perror("Error with sending the package");
        exit(3);
    }
    rec = recv(sock,buffer,BUFFSIZE,0);
    if(rec <= 0){
        perror("Error with receiving the data!");
        exit(4);
    }
    close(sock);
    if(strstr(buffer,"The message has been received.") != NULL){
        free(buffer);
        return 0;
    }
    return 3;
}

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        int len;
        int f = BrowseForOpen();
        char *valasz = ReadPixels(f, &len);
        char *eredmeny = Unwrap(valasz, len);
        char *neptun = "GN6W3I";
        int post = Post(neptun, eredmeny, len);
        if(post == 0){
            printf("\033[1;32m");
            puts("");
            puts("The text is sent successfully!");
            puts("");
            printf("\033[0m");
        }
        else{
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
        puts("2021.03.28");
        printf("\033[0m");
        printf("Github link:\t");
        printf("\033[1;36m");
        puts("https://github.com/bzsol/Cproj/");
        printf("\033[0m");
        puts("");
        /*printf("\033[1;31m");
        printf("▇▇▇▇▇▇▇▇▇▇▇▇\n");
        printf("\033[1;37m");
        printf("▇▇▇▇▇▇▇▇▇▇▇▇\n");
        printf("\033[1;32m");
        printf("▇▇▇▇▇▇▇▇▇▇▇▇\n\n");
        printf("\033[0m");
        */
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
        printf("--testarray\n");
        printf("\033[0m");
        printf("This command show one of the marvelous Hungarian words you can find in your grammar book.\n");
        printf("\033[1;32m");
        printf("./program xxxx.bmp\n");
        printf("\033[0m");
        printf("You will be able to decrypt the professor secret message.\n");
    }
    else if (argc == 2 && (strcmp("--testarray", argv[1]) == 0))
    {
        srand(time(NULL));
        int len;
        char *mem = TestArray(&len);
        char *valasz = Unwrap(mem, len);
        puts(valasz);
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
        char *neptun = "GN6W3I";
        int post = Post(neptun, eredmeny, len);
        if(post == 0){
            printf("\033[1;32m");
            puts("");
            puts("The text is sent successfully!");
            puts("");
            printf("\033[0m");
        }
        else{
            printf("\033[1;31m");
            puts("");
            puts("The text is not delivered!");
            puts("");
            printf("\033[0m");
        }
        //puts(eredmeny);
        free(eredmeny);
    }
    else
    {
        printf("Too many arguements!\nTry again!\n");
    }
    return 0;
}
