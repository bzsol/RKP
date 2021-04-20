#ifndef printf
#include <stdio.h>
#endif
#ifndef free
#include <stdlib.h>
#endif
#ifndef strcmp
#include <string.h>
#endif
#include <unistd.h>
#include <sys/stat.h>
#ifndef open
#include <fcntl.h>
#endif
#include <dirent.h>
#ifndef opendir
#include <unistd.h>
#endif
#ifndef connect
#include <sys/socket.h>
#endif
#ifndef gethostbyname
#include <netdb.h>
#endif
#ifndef INADDR_ANY
#include <netinet/in.h>
#endif
#include <arpa/inet.h>
#include <omp.h>
#ifndef signal
#include <signal.h>
#endif
#define TCP 80
#define BUFFSIZE 1024

/**
 *  @function mallocFail
 * 
 *  Memórialefoglalás probléma/hiba esetén ez az eljárás hívódik meg
 *  @exit 1 -> Memóriafoglalás meghiúsult
 */
void mallocFail()
{
    fprintf(stderr,"Memory allocation failed!");
    exit(1);
}

/**
 *  @function WhatToDo
 * 
 *  Szignál és Alarm kezelés
 *  Ha a felhasználó a titkosított pixelek kikódolása közben ki akar lépni a CTRL+C kombinációt figyelmen kívül hagyva és figyelmeztetve a felhasználót.
 *  Ha ez a kicsomagolás 1 sec-nál hosszabb volt akkor bezárja a programot.
 *  @param sig[in] Ez a beérkező szignál értéke.
 *  @exit 9 -> Túl sok ideig kódolt ki az Unwrap függvény
 * 
 */
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
/**
 *  @function Unwrap
 *  
 *  Titkosított pixelek visszafejtése szöveggé.
 *  első pixel -> utolsó 2 érték 
 *  második és harmadik pixel -> utolsó 3 érték
 *  @param Pbuff[in] -> Pixel Buffer azaz a titkosított pixelekre mutató lefoglalt memóriaterület.
 *  @param NumCh[out] -> Titkosított szöveg karakterszámának memóriacíme.
 *  @return str -> Titkosított szöveg
 *  @exit 1 -> Memóriafoglalás meghiúsult
*/

char *Unwrap(char *Pbuff, int NumCh)
{
    char *str = (char *)calloc(NumCh+1, sizeof(char));
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
    str[NumCh] = '\0'; // It is essential to close the string :) 
    return str;
}

/**
 *   @function ReadPixels
 *   
 *   A binárisan beolvasott fájlt először ellenőrzi hogy ténylegesen BMP majd ezután beolvassa a titkosított pixeleket.
 *   @param f[in] -> binárisan beolvasott file
 *   @param NumCh[out] -> Titkosított szöveg karakterszámának memóriacíme.
 *   @return pixel -> Titkosított pixelekre mutató memóriaterület
 *   @exit 2 -> Az adott fájl nem BMP
 *   @exit 1 -> Memóriafoglalás meghiúsult
*/
char *ReadPixels(int f, int *NumCh)
{
    char *bm = (char *)calloc(2, sizeof(char));
    int offset; // Honnan
    read(f, bm, 2);
    if (strcmp(bm, "BM") != 0)
    {
        fprintf(stderr,"This is not a valid BMP file!");
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
/**
 *  @function BrowseForOpen
 *  Egy kezdetleges és primitív filekezelő felület a felhasználó részére
 *  @return f -> Beolvasott bináris file
 *  @exit 1 -> Memóriafoglalás meghiúsult
 * 
 * 
 */
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
            /*fgets(input,sizeof(input),stdin);
            if(input != NULL && input[strlen(input)-1] == '\n'){
                input[strlen(input)-1] = '\0';
            }*/ // It would be safer but buggy as hell!
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
/**
 *  @function Post
 *  Ez a függvény azért felel hogy a kikódolt üzenetet egy webszerverre küldi.
 *  @param neptunID
 *  @param message
 *  @param NumCh
 *  @exit 1 -> Memóriafoglalás meghiúsult
 *  @exit 3 -> Az adott webszerver cím nem létezik
 *  @exit 4 -> Socket létrehozása nem sikerült
 *  @exit 5 -> A szerverrel való kapcsolat nem történt meg megfelelően
 *  @exit 6 -> A HTTP POST csomag elküldésekor probléma történt
 *  @exit 7 -> Visszaérkező csomag nem érkezett meg vagy valami hiba történt
 *  @return 8 -> A kikódolt szöveg elküldése és visszaérkező üzenet nem egyezett meg a HTTP 200-as kóddal.
 *  @return 0 -> Minden rendben történt az üzenet sikeresen megérkezett.
 *  @const -> TCP -> 80 port
 *  @const -> BUFFER 1024
 * 
 */
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
        fprintf(stderr,"Error! The host is not found!");
        exit(3);
    }
    sprintf(buffer, "POST /~vargai/post.php HTTP/1.1\r\nHost: irh.inf.unideb.hu\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nNeptunID=%s&PostedText=%s", NumCh + 6 + 9 + 12, neptunID, message);
    sock = socket(AF_INET, SOCK_STREAM, 0); // -> Make the socket
    if (sock < 0)
    {
        fprintf(stderr,"Error! Creating the socket is failed!");
        exit(4);
    }
    address.sin_family = AF_INET;  // Ipv4
    address.sin_port = htons(TCP); // define 80 port
    address.sin_addr.s_addr = inet_addr(IP);
    con = connect(sock, (struct sockaddr *)&address, sizeof(address)); // Connect to the server
    if (con < 0)
    {
        fprintf(stderr,"Connection with the server is not established!");
        exit(5);
    }
    //write(sock, buffer, strlen(buffer)); -> küld a szerverre
    sent = send(sock, buffer, strlen(buffer), 0); // -> send() is ugyanaz
    if (sent <= 0)
    {
        fprintf(stderr,"Error with sending the package");
        exit(6);
    }
    rec = recv(sock, buffer, BUFFSIZE, 0);
    if (rec <= 0)
    {
        fprintf(stderr,"Error with receiving the data!");
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