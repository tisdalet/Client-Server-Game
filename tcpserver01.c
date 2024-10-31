//#include "unp.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h> // for read, write
#include <arpa/inet.h> // for sockets
#include <time.h> // time() used to provide a seed for srand() to seed rand()

#define MAXLINE 4096
#define BOARD_SIZE 5
#define LISTENQ 1024

typedef struct
{
	int row;
	int col;
} Guess;

typedef struct {
    char grid[BOARD_SIZE][BOARD_SIZE];
} Board;

typedef struct
{
	int row;
	int col;
	int hitcounter;
} horizontalship;


int main(int argc,char **argv) {
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

    for ( ; ; ) {
        clilen = sizeof(cliaddr);

        // Accept connection
        connfd = accept(listenfd,(SA *)&cliaddr,&clilen);
		
        // Fork child process
        if ((childpid = fork()) == 0) {        /* child process */
            Guess clientguess; // message from client
            char result[MAXLINE]; // for response
			char sunkmessage[MAXLINE];
            ssize_t n;
			horizontalship ship1;

			strcpy(sunkmessage, "Ship sunk!\n");

			// make ship values
			ship1.row = rand() % 5;
			ship1.col = rand() % 3;
			ship1.hitcounter =0;

			// This is for testing
			printf("Ship position: row = %d, col = %d, hitcounter = %d\n", ship1.row, ship1.col, ship1.hitcounter);

            close(listenfd); /* close listening socket */
            while ((n = read(connfd, &clientguess,sizeof(clientguess))) > 0) {
				printf("Guess position: row = %d, col = %d", clientguess.row, clientguess.col);
				if (clientguess.row == ship1.row) { // check if in the same row
					for(int i = ship1.col; i < ship1.col + 2; i++){
						if (clientguess.col == ship1.col) {
							strcpy(result, "Hit!\n");
							ship1.hitcounter++;
							break;
						}
					}
					strcpy(result, "Miss!\n");
				}
				else {
					strcpy(result, "Miss!\n");
				}
				if (ship1.hitcounter == 3) {
					write(connfd, sunkmessage, strlen(sunkmessage));
				}

                // write to client socket
                write(connfd,result,strlen(result));
            }
            close(connfd);
            exit(0);
        }
        close(connfd);  /* parent closes connected socket */
        wait(NULL);
    }
}
