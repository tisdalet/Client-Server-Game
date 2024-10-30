#include	"unp.h"
#include	"unp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for read, write
#include <arpa/inet.h> // for sockets

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;
	char	sendline[MAXLINE], recvline[MAXLINE];


	if (argc != 2){
		printf("usage: tcpcli <IPaddress>");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(23456);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	// str_cli(stdin, sockfd); //might not use this anymore
	while (fgets(sendline, MAXLINE, fp) != NULL) {

		write(sockfd, sendline, strlen(sendline));

		if (read(sockfd, recvline, MAXLINE) == 0){
			printf("str_cli: server terminated prematurely");
			exit(1);
		}

		fputs(recvline, stdout);
	}

	exit(0);
}
