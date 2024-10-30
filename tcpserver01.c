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
            char buf[MAXLINE]; // message from client
            char buf2[MAXLINE]; // for reponse
            ssize_t n;
            close(listenfd); /* close listening socket */
            while ((n = read(connfd,buf,MAXLINE)) > 0) {
                //n = read(connfd, buf, MAXLINE);
                if (n > 0) {
                    // remove end char
                    buf[n] = '\0';
                    // print
                    fputs(buf,stdout);
                }
                // take in front keyboard, stdin 
                fgets(buf2,MAXLINE,stdin);
                // write to socket to client
                write(connfd,buf2,strlen(buf2));
            }
            close(connfd);
            exit(0);
        }
        close(connfd);  /* parent closes connected socket */
        wait(NULL);
    }
}
