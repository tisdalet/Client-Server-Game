//#include "unp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for read, write
#include <arpa/inet.h> // for sockets

int main(int argc,char **argv) {
	typedef struct sockaddr SA;
	int sockfd;
	struct sockaddr_in servaddr;
	FILE *fp;
	typedef struct sockaddr SA;
	char sendline[4096],recvline[4096];

	if (argc != 2) {
		printf("usage: tcpcli <IPaddress>");
		exit(1);
	}

	sockfd = socket(2,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = 2;
	servaddr.sin_port = htons(23456);
	inet_pton(2,argv[1],&servaddr.sin_addr);
	connect(sockfd,(SA *)&servaddr,sizeof(servaddr));

	// str_cli(stdin,sockfd); //might not use this anymore
	while (fgets(sendline,4096,fp) != NULL) {
		write(sockfd,sendline,strlen(sendline));
		if (read(sockfd,recvline,4096) == 0) {
			printf("str_cli: server terminated prematurely");
			exit(1);
		}
		fputs(recvline,stdout);
	}
	exit(0);
}
