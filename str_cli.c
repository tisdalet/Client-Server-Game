#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define MAXLINE 4096

void str_cli(FILE *fp, int sockfd) {
    char sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        write(sockfd, sendline, strlen(sendline));

        if (read(sockfd, recvline, MAXLINE) == 0) {
            printf("str_cli: server terminated prematurely");
            exit(1);
        }

        fputs(recvline, stdout);
    }
}
