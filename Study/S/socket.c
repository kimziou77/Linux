#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_LQ_SIZE 5
#define MAX_NAME_SIZE 64
#define MAX_BUF_SIZE 1024
#define PERMS 0666

void errorHandler(const char *msg);
void signalHandler(int signum);

int sockfd = 0;
int clntSock = 0;
int main(int argc, char const *argv[]) {
    int fileFd = 0;
    char fileName[MAX_NAME_SIZE + 1] = {
        0x00,
    };
    size_t fileSize = 0;

    char fileData[MAX_BUF_SIZE] = {
        0x00,
    };
    size_t recvSize = 0;
    size_t totalSize = 0;

    struct sockaddr_in servAddr = {
        0x00, // server address info
    };
    struct sockaddr_in clntAddr = {
        0x00, // client address info
    };
    int clntAddrSize = 0;
    signal(SIGINT, signalHandler);
    signal(SIGPIPE, signalHandler);
    if (argc < 2) {
        printf("Usage : %s [port]", argv[0]);
        return -1;
    }
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == 1) {
        errorHandler("sock() error!");
    }

    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if (bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr))) {
        errorHandler("bind() error!");
    }
    if (listen(sockfd, MAX_LQ_SIZE) == -1) {
        errorHandler("listen() error!");
    }
    while (1) {
        puts("Wait ...");
        if ((clntSock = accept(sockfd, (struct sockaddr *)&clntAddr,
                               &clntAddrSize)) == -1) {
            errorHandler("accept() error!");
        }
        printf("Client IP : %s \n", inet_ntoa(clntAddr.sin_addr));

        while (1) {
            memset(fileName, 0x00, sizeof(fileName));
            recv(clntSock, fileName, MAX_NAME_SIZE, 0);

            if (strcmp(fileName, "quit") == 0) {
                close(clntSock);
                break;
            }
            recv(clntSock, &fileSize, sizeof(size_t), 0);
            printf(">> File name : %s, size : %d byte\n", fileName,
                   (int)fileSize);
            fileFd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, PERMS);
            totalSize = 0;
            while (1) {
                if (totalSize != fileSize) {
                    memset(fileData, 0x00, sizeof(fileData));
                    recvSize = recv(clntSock, fileData, MAX_BUF_SIZE, 0);
                    write(fileFd, fileData, recvSize);
                    totalSize += (size_t)recvSize;
                } else {
                    close(fileFd);
                    puts(">> Success!");
                    break;
                }
            }
        }
        close(clntSock);
    }

    return 0;
}
void errorHandler(const char *msg) {
    close(clntSock);
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
