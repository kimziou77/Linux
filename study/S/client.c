#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_NAME_SIZE 64
#define MAX_BUF_SIZE 1024
void errorHandler(const char *msg);
void signalHandler(int signum);
int sockfd = 0;

int main(int argc, char const *argv[]) {
    int fileFd;
    char fileName[MAX_NAME_SIZE + 1];
    size_t fileSize = 0;

    char fileData[MAX_BUF_SIZE] = {
        0x00,
    };
    size_t readSize = 0;
    size_t totalSize = 0;
    struct stat fileStat;
    struct sockaddr_in servAddr = {
        0x00,
    };
    signal(SIGINT, signalHandler);
    signal(SIGPIPE, signalHandler);
    if (argc != 3) {
        printf("Usage : %s [server IP] [port]\n", argv[0]);
        exit(1);
    }
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        errorHandler("sock() error!");
    }
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1) {
        errorHandler("connect() error!");
    }
    while (1) {
        printf("<< Please input the name of file(\"quit\" to exit): ");
        memset(fileName, 0x00, sizeof(fileName));
        scanf("%s", fileName);
        if (strcmp(fileName, "quit") == 0) {
            send(sockfd, fileName, strlen(fileName), 0);
            break;
        }
        if ((fileFd = open(fileName, O_RDONLY)) == -1) {
            perror("open() error!");
            continue;
        }
        send(sockfd, fileName, strlen(fileName), 0);
        fstat(fileFd, &fileStat);
        send(sockfd, &(fileStat.st_size), sizeof(size_t), 0);

        while (1) {
            memset(fileData, 0x00, sizeof(fileData));
            readSize = read(fileFd, fileData, MAX_BUF_SIZE);
            if (readSize == 0)
                break;
            send(sockfd, fileData, readSize, 0);
        }
        close(fileFd);
    }
    close(sockfd);
    return 0;
}

void errorHandler(const char *msg) {
    close(sockfd);
    fputs(msg, stderr);
    exit(-1);
}
void signalHandler(int signum) {
    if (signum == SIGINT) {
        close(sockfd);
        exit(-1);
    }
    if (signum == SIGPIPE) {
        puts("disconnected!");
        exit(-1);
    }
}
