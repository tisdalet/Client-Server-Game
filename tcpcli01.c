//#include "unp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for read, write
#include <arpa/inet.h> // for sockets
#include <time.h> // time() used to provide a seed for srand() to seed rand()
#include <stdbool.h> // gameover variable
#include <ctype.h> // for tolower()

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
	for(int i = 0; i < BOARD_SIZE; i++)
	{
		for(int j = 0; j < BOARD_SIZE; j++)
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

	int sunk = 0, turn = 0; // counters
	int ammocount = 11;
	bool gameover;
	while (ammocount > 0 && !gameover) {
		turn++;
		printf("\n| AMMO %d | TURN %d |\n\n", ammocount, turn); // display turn and score counters
		printf("BATTLE SHIP BOARD\n");
		printf("    A  B  C  D  E\n");
		printf("    -  -  -  -  -\n");
		for(int i = 0; i < BOARD_SIZE; i++) {
			printf("%d |", i + 1);
			for(int j = 0; j < BOARD_SIZE; j++)
			{
				printf(" %d ", gameboard.grid[i][j]);
			}
			printf("\n");
		}
		printf("Enter your guess (row and col): ");
		int row, col;
		char colchar;

		scanf("%d %c", &row, &colchar);
		// Switch to handle the char
		colchar = tolower(colchar);
		// printf("GUESS CHAR %c", colchar);
		switch (colchar) {
			case 'a':
				col = 0;
				break;
			case 'b':
				col = 1;
				break;
			case 'c':
				col = 2;
				break;
			case 'd':
				col = 3;
				break;
			case 'e':
				col = 4;
				break;
			default:
				printf("Other character entered. Bad.\n");
				break;
    		}
		// printf("Row is %d", col);
		Guess guess;
		guess.row = row - 1;
		guess.col = col;

		write(sockfd, &guess, sizeof(guess));;
		// Read for hit or miss.
		if (read(sockfd,recvline,4096) == 0) {
			printf("str_cli: server terminated prematurely\n");
			exit(1);
		}
		// receiving "Win!" or "Lose!" from the server will terminate the client game loop
			/* second condition will catch reattempts where we previously hit a ship and updated 
		a 2 on the client-side gameboard. It just sets a 2, effectively keeping that location as a 2*/
		if (strstr(recvline, "Hit!") != NULL || gameboard.grid[row - 1][col] == 2)
		{
			gameboard.grid[row - 1][col] = 2; // 2 = hit
		} else {
			gameboard.grid[row - 1][col] = 1; // 1 = miss
		}
		if(strstr(recvline, "Ship sunk!") != NULL) // when a ship is sunk
		{
			ammocount += 3;
			printf("+2 AMMO \n");
		}
		fputs(recvline, stdout);
		memset(recvline, 0, sizeof(recvline));
		read(sockfd, recvline, 4096);
		if(strstr(recvline, "Win!") != NULL || strstr(recvline, "Lose!") != NULL)
		{
			fputs(recvline, stdout);
			gameover = true;
		}
		memset(recvline, 0, sizeof(recvline));
		ammocount--; // reduce ammo at the end of each turn
	}
	exit(0);
}
