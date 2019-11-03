#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char const *argv[]) {
    char *ip = "127.0.0.1";                      // loop back host name
    printf("IP : %s\n", ip);                     // string ip
    printf("Converted IP; %d\n", inet_addr(ip)); // int ip
    // inet_addr(ip) : string ip address->int address

    /*  return value  */
    // success : int ip
    // fail    : -1
    return 0;
}
