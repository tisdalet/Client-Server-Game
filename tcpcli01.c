//#include "unp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for read, write
#include <arpa/inet.h> // for sockets
#include <time.h> // time() used to provide a seed for srand() to seed rand()

#define BOARD_SIZE 5

struct guesses
{
	int row;
	int col;
	int hit;
};
typedef struct {
    char grid[BOARD_SIZE][BOARD_SIZE];
} Board;

int main(int argc,char **argv) {
	typedef struct sockaddr SA;
	int sockfd;
	struct sockaddr_in servaddr;
	FILE *fp = stdin;
	typedef struct sockaddr SA;
	char sendline[4096],recvline[4096];
	Board gameboard;

	if (argc != 2) {
		printf("usage: tcpcli <IPaddress>");
		exit(1);
	}

	// Initialize the gameboard to 0s
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			gameboard.grid[i][j] = 0;
		}
	}

	// Print the board
	printf("BATTLE SHIP BOARD\n");
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			printf(" %d ", gameboard.grid[i][j]);
		}
		printf("\n");
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
