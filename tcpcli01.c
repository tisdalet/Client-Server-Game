//#include "unp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for read, write
#include <arpa/inet.h> // for sockets
#include <time.h> // time() used to provide a seed for srand() to seed rand()

#define BOARD_SIZE 5

typedef struct
{
	int row;
	int col;
} Guess;

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

	sockfd = socket(2,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = 2;
	servaddr.sin_port = htons(23456);
	inet_pton(2,argv[1],&servaddr.sin_addr);
	connect(sockfd,(SA *)&servaddr,sizeof(servaddr));

	int ammocount = 0;
	while (ammocount < 11) {
		printf("BATTLE SHIP BOARD\n");
		for(int i = 0; i < 5; i++)
		{
			for(int j = 0; j < 5; j++)
			{
				printf(" %d ", gameboard.grid[i][j]);
			}
			printf("\n");
		}
		printf("Enter your guess (row and col): ");
		int row, col;

		scanf("%d %d", &row, &col);
		Guess guess;
		guess.row = row - 1;
		guess.col = col - 1;

		write(sockfd, &guess, sizeof(guess));;
		// Read for hit or miss.
		if (read(sockfd,recvline,4096) == 0) {
			printf("str_cli: server terminated prematurely\n");
			exit(1);
		}
		if (strstr(recvline, "Hit!") != NULL || gameboard.grid[row - 1][col - 1] == 2) // second condition will catch reattempts where we previously hit a ship and updated a 2 on the client-side gameboard. It just sets a 2, effectively keeping that location as a 2
		{
//			gameboard.grid[guess.row][guess.col] = 2;
	//		printf("lol\n");
			gameboard.grid[row - 1][col -1] = 2; // 2 = hit
		} else {
			gameboard.grid[row - 1][col - 1] = 1; // 1 = miss
		}
		fputs(recvline,stdout);
		memset(recvline, 0, sizeof(recvline));
		ammocount++;
	}
	exit(0);
}
