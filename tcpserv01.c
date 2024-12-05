//#include "unp.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h> // for read, write
#include <arpa/inet.h> // for sockets
#include <time.h> // time() used to provide a seed for srand() to seed rand()
#include <stdbool.h> // 'ships_placed' variable, 'sunk' data member of Horizontalship struct

#define MAXLINE 4096
#define BOARD_SIZE 5
#define LISTENQ 1024

typedef struct
{ // holds coordinates of guess received from client
	int row;
	int col;
} Guess;

typedef struct
{ // gameboard struct that holds 2D matrix for displaying ship positions
	char grid[BOARD_SIZE][BOARD_SIZE];
} Board;

typedef struct
{ // ship struct for holding the details of each ship on the board
	int row[2];
	int col[2];
	int hitcounter;
	bool sunk;
} Horizontalship;

void place_ships(Board *board, int amount_of_ships, Horizontalship *ships)
{
	for(int i = 0; i < amount_of_ships; i++)
	{
		bool ship_placed = false; // each ship's placement is false until all its points are placed
		while(!ship_placed) // while loop until a ship's placement is complete
		{
			ships[i].row[0] = rand() % BOARD_SIZE; // random row index value between 0 and 4 stored in the first coordinate position of a ship
			ships[i].col[0] = rand() % BOARD_SIZE; // random column index value between 0 and 4 stored in the first coordinate position of a ship
			ships[i].hitcounter = 0; // start each ship struct's hitcounter at 0
			int row = ships[i].row[0]; // store the randomly generated value in a row variable for convenience
			int col = ships[i].col[0]; // store the randomly generated value in a col variable for convenience
			if(board->grid[row][col] != 1) // if the starting coordinate isn't already a 1 on the gameboard, continue. Otherwise, the while loop will restart
			{
				if(row + 1 < BOARD_SIZE && board->grid[row + 1][col] != 1) // if the next row doesn't exceed the gameboard size, and the cell of the next row isn't already a 1 on the gameboard, continue
				{
					ships[i].row[0] = row; // store the row of the ship's origin to its struct
					ships[i].col[0] = col; // store the col of the ship's origin to its struct
					ships[i].row[1] = row + 1; // store the row of the 2nd coordinate of the ship to its struct
					ships[i].col[1] = col; // store the row of the 2nd coordinate of the ship to its struct
					board->grid[row][col] = board->grid[row + 1][col] = 1; // assign both sets of coordinates with a 1 on the gameboard
					ship_placed = true; // ship was successfully placed, so we'll break the while loop and iterate to the next ship to be placed
				}

				else if(col + 1 < BOARD_SIZE && board->grid[row][col + 1] != 1) // SEE ABOVE
				{
					ships[i].row[0] = row; // SEE ABOVE
					ships[i].col[0] = col; // SEE ABOVE
					ships[i].row[1] = row; // SEE ABOVE
					ships[i].col[1] = col + 1; // SEE ABOVE
					board->grid[row][col] = board->grid[row][col + 1] = 1; // SEE ABOVE
					ship_placed = true; // SEE ABOVE
				}

				else if(row - 1 >= 0 && board->grid[row - 1][col] != 1) // SEE ABOVE
				{
					ships[i].row[0] = row; // SEE ABOVE
					ships[i].col[0] = col; // SEE ABOVE
					ships[i].row[1] = row - 1; // SEE ABOVE
					ships[i].col[1] = col; // SEE ABOVE
					board->grid[row][col] = board->grid[row - 1][col] = 1; // SEE ABOVE
					ship_placed = true; // SEE ABOVE
				}

				else if(col - 1 >= 0 && board->grid[row][col - 1] != 1) // SEE ABOVE
				{
					ships[i].row[0] = row; // SEE ABOVE
					ships[i].col[0] = col; // SEE ABOVE
					ships[i].row[1] = row; // SEE ABOVE
					ships[i].col[1] = col - 1; // SEE ABOVE
					board->grid[row][col] = board->grid[row][col - 1] = 1; // SEE ABOVE
					ship_placed = true; // SEE ABOVE
				}
			}

		}
	}
}

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
	for(int i = 0; i < BOARD_SIZE; i++)
	{
		for(int j = 0; j < BOARD_SIZE; j++)
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
			bool all_ships_sunk;
			ssize_t n;

			// create 3 ship struct objects and store in array
			int total_ships = 3;
			Horizontalship ships[total_ships];

			// load the sunk string to sunkmessage variable
			strcpy(sunkmessage, "Ship sunk!\n");

			// make ship values
			place_ships(&gameboard, total_ships, ships);
			
			write(connfd, result, strlen(result));
			// This is for testing
			//printf("Ship position: row = %d, col = %d, hitcounter = %d\n", ship1.row + 1, ship1.col + 1, ship1.hitcounter);
			// Print the board
			printf("BATTLE SHIP BOARD\n");
			for(int i = 0; i < BOARD_SIZE; i++)
			{
				for(int j = 0; j < BOARD_SIZE; j++)
				{
					printf(" %d ", gameboard.grid[i][j]);
				}
				printf("\n");
			}

			close(listenfd); /* close listening socket */
			while ((n = read(connfd, &clientguess,sizeof(clientguess))) > 0 && !all_ships_sunk)
			{
				printf("Guess position: row = %d, col = %d\n", clientguess.row + 1, clientguess.col + 1); // display client's guess
				strcpy(result, "Miss!\n"); // assume a miss until otherwise

				for(int i = 0; i < total_ships; i++) // iterate over each ship to check for hits
				{
					for(int j = 0; j < 2; j++) // itereate over each coordinate of the ship
					{
						if(ships[i].row[j] == clientguess.row && ships[i].col[j] == clientguess.col && gameboard.grid[clientguess.row][clientguess.col] == 1) // check for matching coordinates and then check the value of the coordinate on the gameboard when a matching set is found
						{ // continue if a 1 is detected at the guessed coordinates

						//	printf("ALERT: We've been hit!\n");
							strcpy(result, "Hit!\n"); // store Hit!\n in the result variable to be sent over socket
							//write(connfd, result, strlen(result));
							gameboard.grid[clientguess.row][clientguess.col] = 9; // mark cell as hit (randomly chose 9 at this moment)

							ships[i].hitcounter++; // increment the ship struct's hitcounter
							if(ships[i].hitcounter == 2) // if that hitcounter is 2, ship sunk!
							{
								ships[i].sunk = true; // update struct of sunk ship
								// write the ship sunk message to connected socket
								// (probably don't need to specify the content of the sunk message, as we did that near the top of this program)
								write(connfd, sunkmessage, strlen(sunkmessage)); 
							}
							if (!ships[i].sunk) {
								break;
							}
							else if (i == (total_ships - 1)){
								strcpy(result, "Win!\n");
							}

							break; // break out of loop when matching coordinates found
						}
					}
				}
				write(connfd, result, strlen(result));
				// Print the board
				printf("BATTLE SHIP BOARD\n");
				for(int i = 0; i < BOARD_SIZE; i++)
				{
					for(int j = 0; j < BOARD_SIZE; j++)
					{
						printf(" %d ", gameboard.grid[i][j]);
					}
					printf("\n");
				}


				for(int i = 0; i < total_ships; i++) // loop that checks if all ships are sunk (victory condition)
				{
					if(!ships[i].sunk) break;
					else if(i == (total_ships - 1))
					{

						printf("THE COMMIES SANK US!\n");
						all_ships_sunk = true;
					}
				}

			} // end main game (while) loop
		}
        close(connfd);  /* parent closes connected socket */
		wait(NULL);
	}
}