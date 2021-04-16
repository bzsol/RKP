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
#include <omp.h>
#include <signal.h>
#define TCP 80
#define BUFFSIZE 1024

void mallocFail()
{
    perror("The memory allocation is failed!");
    exit(1);
}

void WhatToDo(int sig)
{
    if (sig == SIGINT)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            puts("The CTRL+C has been disabled!");
            kill(getpid(), SIGKILL);
        }
    }
    else
    {
        // perror usage is not obligatory
        fprintf(stderr, "Timeout happened the program is closed!");
        exit(9);
    }
}

char *Unwrap(char *Pbuff, int NumCh)
{
    char *str = (char *)calloc(NumCh, sizeof(char));
    if (str == NULL)
    {
        mallocFail();
    }
    int i = 0;
    int j = 0;
    char buffer;
#pragma omp parallel for private(buffer, i, j) shared(str)
    for (i = 0; i < NumCh; i++)
    {
        buffer = 0;
        for (j = 0; j < 3; j++)
        {
            if (j == 0)
            {
                buffer = Pbuff[i * 3 + j] << 6;
            }
            else if (j == 1)
            {
                buffer |= ((Pbuff[i * 3 + j] << 3) & 56);
            }
            else if (j == 2)
            {
                buffer |= (Pbuff[i * 3 + j] & 7);
            }
        }
        str[i] = buffer;
    }
    free(Pbuff);
    return str;
}

char *ReadPixels(int f, int *NumCh)
{
    char *bm = (char *)calloc(2, sizeof(char));
    int offset; // Honnan
    read(f, bm, 2);
    if (strcmp(bm, "BM") != 0)
    {
        perror("This is not a valid BMP file!");
        free(bm);
        exit(2);
    }
    else if (bm == NULL)
    {
        mallocFail();
    }
    free(bm);
    lseek(f, 6, SEEK_SET); // 6. byte-tól megyünk
    read(f, NumCh, 4);
    read(f, &offset, 4);
    char *pixel = (char *)malloc((*NumCh * 3) * sizeof(char)); // R G B
    if (pixel == NULL)
    {
        mallocFail();
    }
    lseek(f, offset, SEEK_SET);
    read(f, pixel, *NumCh * 3);
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
    if (filename == NULL || path == NULL || input == NULL)
    {
        mallocFail();
    }
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
    char *buffer = (char *)malloc(BUFFSIZE * sizeof(char)); // 1kB adat
    if (buffer == NULL)
    {
        mallocFail();
    }
    char *received;
    hostinfo = gethostbyname(IP);
    if (hostinfo == NULL)
    {
        perror("Error! The host is not found!");
        exit(3);
    }
    sprintf(buffer, "POST /~vargai/post.php HTTP/1.1\r\nHost: irh.inf.unideb.hu\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nNeptunID=%s&PostedText=%s", NumCh + 6 + 9 + 12, neptunID, message);
    sock = socket(AF_INET, SOCK_STREAM, 0); // -> Make the socket
    if (sock < 0)
    {
        perror("Error! Creating the socket is failed!");
        exit(4);
    }
    address.sin_family = AF_INET;  // Ipv4
    address.sin_port = htons(TCP); // define 80 port
    address.sin_addr.s_addr = inet_addr(IP);
    con = connect(sock, (struct sockaddr *)&address, sizeof(address)); // Connect to the server
    if (con < 0)
    {
        perror("Connection with the server is not established!");
        exit(5);
    }
    //write(sock, buffer, strlen(buffer)); -> küld a szerverre
    sent = send(sock, buffer, strlen(buffer), 0); // -> send() is ugyanaz
    if (sent <= 0)
    {
        perror("Error with sending the package");
        exit(6);
    }
    rec = recv(sock, buffer, BUFFSIZE, 0);
    if (rec <= 0)
    {
        perror("Error with receiving the data!");
        exit(7);
    }
    close(sock);
    if (strstr(buffer, "The message has been received.") != NULL)
    {
        free(buffer);
        return 0;
    }
    free(buffer);
    return 8;
}