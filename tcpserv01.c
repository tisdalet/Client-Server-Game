//#include "unp.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h> // for read, write
#include <arpa/inet.h> // for sockets
#include <time.h> // time() used to provide a seed for srand() to seed rand()
#include <stdbool.h> // ships_placed boolean variable

#define MAXLINE 4096
#define BOARD_SIZE 5
#define LISTENQ 1024

typedef struct
{
	int row;
	int col;
} Guess;

typedef struct
{
	char grid[BOARD_SIZE][BOARD_SIZE];
} Board;

typedef struct
{
	int row;
	int col;
	int hitcounter;
} Horizontalship;


int main(int argc,char **argv)
{
	int listenfd,connfd;
    	pid_t childpid;
    	socklen_t clilen;
   	struct sockaddr_in cliaddr,servaddr;
   	typedef struct sockaddr SA;
   	Board gameboard;
	srand(time(0)); // seed random number generation with number of seconds since January 1, 1970

	listenfd = socket(AF_INET,SOCK_STREAM,0);

    	bzero(&servaddr,sizeof(servaddr));
    	servaddr.sin_family = AF_INET;
    	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    	servaddr.sin_port = htons(23456);
	bind(listenfd,(SA *)&servaddr,sizeof(servaddr));

	listen(listenfd,LISTENQ);
	printf("Server is ready to talk...\n");
	// Initialize the gameboard to 0s
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			gameboard.grid[i][j] = 0;
		}
	}

	for(;;)
	{
        	clilen = sizeof(cliaddr);

        	// Accept connection
        	connfd = accept(listenfd,(SA *)&cliaddr,&clilen);

        	// Fork child process
        	if ((childpid = fork()) == 0)
		{        /* child process */
            		Guess clientguess; // message from client
            		char result[MAXLINE]; // for response
			char sunkmessage[MAXLINE];
			ssize_t n;
			//	Horizontalship ship1;
			Horizontalship ships[3];

			strcpy(sunkmessage, "Ship sunk!\n");

			// make ship values
			bool ships_placed = false;
			while(!ships_placed)
			{
				for(int i = 0; i < 3; i++)
				{
					ships[i].row = rand() % 5; // random row index value between 0 and 4
					ships[i].col = rand() % 3; // random column index value between 0 and 2 (temporary constraint for testing?)
					ships[i].hitcounter = 0;
					gameboard.grid[ships[i].row][ships[i].col] = 1;
					if (i == 2){
						ships_placed = true;
					}
					// ships_placed = true;
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

			//	printf("Ship position: row = %d, col = %d, hitcounter = %d\n", ship1.row + 1, ship1.col + 1, ship1.hitcounter);
			// print ship positions, for testing.
			for (int i = 0; i < 3; i++) {
				printf("Ship%d position: row = %d, col = %d, hitcounter = %d\n", i, ships[i].row + 1, ships[i].col + 1, ships[i].hitcounter);
			}


			close(listenfd); /* close listening socket */
			while ((n = read(connfd, &clientguess,sizeof(clientguess))) > 0)
			{
				printf("Guess position: row = %d, col = %d\n", clientguess.row + 1, clientguess.col + 1);
				strcpy(result, "Miss!\n");
				if (gameboard.grid[clientguess.row][clientguess.col] == 1) // check if the client's guess equals a 1 on the gameboard
				{
					strcpy(result, "Hit!\n");
					//	ship1.hitcounter++;
				//	break;
				}
				else
				{
					strcpy(result, "Miss!\n");
				}

                		// write to client socket
			//	printf("IT IS WRITTEN!");
                		write(connfd,result,strlen(result));
				// If ship has been hit 3 times
				//	if (ship1.hitcounter == 3) {
				//		write(connfd, sunkmessage, strlen(sunkmessage));
				//		ship1.hitcounter = 0;
				//	}
			}
			close(connfd);
            		exit(0);
		}
        	close(connfd);  /* parent closes connected socket */
		wait(NULL);
	}
}