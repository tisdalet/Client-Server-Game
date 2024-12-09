#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for read, write
#include <arpa/inet.h> // for sockets
#include <time.h> // time() used to provide a seed for srand() to seed rand()
#include <stdbool.h> // gameover variable
#include <ctype.h> // for tolower(), toupper()
#include <sqlite3.h> // for database

#define BOARD_SIZE 5

typedef struct
{
    int row;
    int col;
} Guess;

typedef struct {
    char grid[BOARD_SIZE][BOARD_SIZE];
} Board;

sqlite3 *db;  // SQLite database

// Initialize the SQLite database
int init_db() {
    int rc = sqlite3_open("./highscores.db", &db); // Open the database file
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Create the high_scores table if it doesn't exist
    const char *sql_create_table = "CREATE TABLE IF NOT EXISTS high_scores (id INTEGER PRIMARY KEY, player_name TEXT, score INTEGER);";

    // create the table
    char *err_msg = 0;
    rc = sqlite3_exec(db, sql_create_table, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    return SQLITE_OK;
}

// Print high scores
void show_highscores() {
    const char *sql_select = "SELECT player_name, score FROM high_scores ORDER BY score DESC LIMIT 5;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch high scores: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\nHigh Scores:\n");
    int rank = 1;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *player_name = (const char *)sqlite3_column_text(stmt, 0);
        int score = sqlite3_column_int(stmt, 1);
        printf("%d. %s - %d points\n", rank++, player_name, score);
    }
    sqlite3_finalize(stmt); // Finalize the statement
}

// Insert a new high score into the database
void insert_highscore(const char *player_name, int score) {
    const char *sql_insert = "INSERT INTO high_scores (player_name, score) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to insert high score: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, player_name, strlen(player_name), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, score);

    rc = sqlite3_step(stmt); // Execute the statement
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt); // Finalize the statement
}

void print_title()
{
	printf("\n");
	printf(" ██████╗  █████╗ ████████╗████████╗██╗     ███████╗    ███████╗██╗  ██╗██╗██████╗\n");
	printf(" ██╔══██╗██╔══██╗╚══██╔══╝╚══██╔══╝██║     ██╔════╝    ██╔════╝██║  ██║██║██╔══██╗\n");
	printf(" ██████╔╝███████║   ██║      ██║   ██║     █████╗      ███████╗███████║██║██████╔╝\n");
	printf(" ██╔══██╗██╔══██║   ██║      ██║   ██║     ██╔══╝      ╚════██║██╔══██║██║██╔═══╝\n");
	printf(" ██████╔╝██║  ██║   ██║      ██║   ███████╗███████╗    ███████║██║  ██║██║██║\n");
	printf(" ╚═════╝ ╚═╝  ╚═╝   ╚═╝      ╚═╝   ╚══════╝╚══════╝    ╚══════╝╚═╝  ╚═╝╚═╝╚═╝\n");
}

int main(int argc, char **argv) {
    typedef struct sockaddr SA;
    int sockfd;
    struct sockaddr_in servaddr;
    FILE *fp = stdin;
    typedef struct sockaddr SA;
    char sendline[4096], recvline[4096];
    Board gameboard;
	char initials[4]; 

    if (argc != 2) {
        printf("usage: tcpcli <IPaddress>");
        exit(1);
    }

    // Make database
    if (init_db() != SQLITE_OK) {
        exit(1); // Exit if the database can't be made
    }

    // Menu for user choice
    int choice;
    print_title();
    do {
    	printf("\n\n");
    	printf("\t 1. Play Game\n");
    	printf("\t 2. View High Scores\n\n");
    	printf("\t Enter your choice: ");
    	scanf("%d", &choice);

    	if (choice == 2) {
        	show_highscores();
    	}
    }
    while(choice != 1);

    // Initialize the gameboard to 0s
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            gameboard.grid[i][j] = 0;
        }
    }

    sockfd = socket(2, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = 2;
    servaddr.sin_port = htons(23456);
    inet_pton(2, argv[1], &servaddr.sin_addr);
    connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

    int sunk = 0, turn = 0; // counters
    int ammocount = 11;
    bool win;
    while (ammocount > 0 && !win) {
        turn++;
        printf("\n| AMMO %d | TURN %d |\n\n", ammocount, turn); // display turn and score counters
        printf("BATTLE SHIP BOARD\n");
        printf("    A  B  C  D  E\n");
        printf("    -  -  -  -  -\n");
        for (int i = 0; i < BOARD_SIZE; i++) {
            printf("%d |", i + 1);
            for (int j = 0; j < BOARD_SIZE; j++) {
                printf(" %d ", gameboard.grid[i][j]);
            }
            printf("\n");
        }
		// Get user guess
        printf("Enter your guess (row and col): ");
        int row, col;
        char colchar;

        scanf("%d %c", &row, &colchar);
        // Switch to handle the char
        colchar = tolower(colchar);
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

        Guess guess;
        guess.row = row - 1;
        guess.col = col;

        write(sockfd, &guess, sizeof(guess));
        if (read(sockfd, recvline, 4096) == 0) {
            printf("str_cli: server terminated prematurely\n");
            exit(1);
        }

        if (strstr(recvline, "Hit!") != NULL || gameboard.grid[row - 1][col] == 2) {
            gameboard.grid[row - 1][col] = 2; // 2 = hit
        } else {
            gameboard.grid[row - 1][col] = 1; // 1 = miss
        }
        if (strstr(recvline, "Ship sunk!") != NULL) {
            ammocount += 3;
            printf("+3 AMMO \n");
        }
        fputs(recvline, stdout);
        memset(recvline, 0, sizeof(recvline));
        read(sockfd, recvline, 4096);
        if (strstr(recvline, "Win!") != NULL || strstr(recvline, "Lose!") != NULL) {
            fputs(recvline, stdout);
            win = true;
	    // Get initials
	    printf("\nHIGH SCORE: Enter your initials (3 characters): ");
            scanf("%3s", initials);
	    for(int i = 0; i < 3; i++)
	    {
		initials[i] = toupper(initials[i]);
	    }

			// Insert int DB ONLY if win
			insert_highscore(initials, ammocount);
        }
        memset(recvline, 0, sizeof(recvline));
        ammocount--; // reduce ammo at the end of each turn
    } // game while loop
    if(!win) printf("\nL O S E R !\n");
    sqlite3_close(db);
    exit(0);
}
