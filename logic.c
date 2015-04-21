#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logic.h"

int mouseRow, mouseCol, catRow, catCol, cheeseX, cheeseY;

void switchTurn() {
	if (!strcmp(turn, "cat")) {
		sprintf(turn, "mouse");
	} else
		sprintf(turn, "cat");

}

int main(int argc, char* args[]) {

	setvbuf(stdout, NULL, _IONBF, 0);
	gameOptions game = { 1, 0, 1, 0 };

	char temp[7];
	char** board; //7*7 board
	board = loadGame(3);
	printf("result %d\n", updateGameStatus(board));
	printf("cat %d %d", catRow, catCol);
	while (!updateGameStatus(board)) {
		printBoard(board);
		if ((game.cat_human && !strcmp(turn, "cat"))
				|| (game.mouse_human && !strcmp(turn, "mouse"))) {
			printf("%s's turn- type a move please (U/D/L/R): \n", turn);
			scanf("%s", temp);
		} //end human move
		else { //machine move
			   //TODO - implement machine move - the result should be put into temp[0] as a character implying direction - L/R/D/U
		}
		switch (temp[0]) {
		case 'L':
			move(&board, LEFT);
			break;
		case 'R':
			move(&board, RIGHT);
			break;
		case 'U':
			move(&board, UP);
			break;
		case 'D':
			move(&board, DOWN);
			break;
		} //end switch

		switchTurn();
	}
	switch (updateGameStatus(board)) {
	case 0:
		printf("Game over. tie.\n");
		break;
	case 1:
		printf("Game over. Mouse wins.\n");
		break;
	case 2:
		printf("Game over. Cat wins. \n");
		break;
	}
	printBoard(board);
//saveGame(board, 3);

	freeBoard(board);
	return 0;
}

int move(char*** board, int direction) {
//returns 1 if move was valid and made, 0 on fail;
	if (!strcmp(turn, "cat")) {
		if (direction == LEFT && catCol > 0
				&& (*board)[catRow][catCol - 1] != 'W'
				&& (*board)[catRow][catCol - 1] != 'P') {
			(*board)[catRow][catCol] = '#';
			catCol--;
			(*board)[catRow][catCol] = 'C';
			return 1;
		}
		if (direction == RIGHT && catCol < 6
				&& (*board)[catRow][catCol + 1] != 'W'
				&& (*board)[catRow][catCol + 1] != 'P') {
			(*board)[catRow][catCol] = '#';
			catCol++;
			(*board)[catRow][catCol] = 'C';
			return 1;
		}
		if (direction == DOWN && catRow < 6
				&& (*board)[catRow + 1][catCol] != 'W'
				&& (*board)[catRow + 1][catCol] != 'P') {
			(*board)[catRow][catCol] = '#';
			catRow++;
			(*board)[catRow][catCol] = 'C';
			return 1;
		}
		if (direction == UP && catRow > 0 && (*board)[catRow - 1][catCol] != 'W'
				&& (*board)[catRow - 1][catCol] != 'P') {
			(*board)[catRow][catCol] = '#';
			catRow--;
			(*board)[catRow][catCol] = 'C';
			return 1;
		}
	}
	if (!strcmp(turn, "mouse")) {
		if (direction == LEFT && mouseCol > 0
				&& (*board)[mouseRow][mouseCol - 1] != 'W') {
			(*board)[mouseRow][mouseCol] = '#';
			mouseCol--;
			(*board)[mouseRow][mouseCol] = 'M';
			return 1;
		}
		if (direction == RIGHT && mouseCol < 6
				&& (*board)[mouseRow][mouseCol + 1] != 'W') {
			(*board)[mouseRow][mouseCol] = '#';
			mouseCol++;
			(*board)[mouseRow][mouseCol] = 'M';
			return 1;
		}
		if (direction == DOWN && mouseRow < 6
				&& (*board)[mouseRow + 1][mouseCol] != 'W') {
			(*board)[mouseRow][mouseCol] = '#';
			mouseRow++;
			(*board)[mouseRow][mouseCol] = 'M';
			return 1;
		}
		if (direction == UP && mouseRow > 0
				&& (*board)[mouseRow - 1][mouseCol] != 'W') {
			(*board)[mouseRow][mouseCol] = '#';
			mouseRow--;
			(*board)[mouseRow][mouseCol] = 'M';
			return 1;
		}
	}
	return 0;
}

void freeBoard(char** board) {
	if (board == NULL)
		return;
	int i = 0;
	for (i = 0; i < 7; i++) {
		free(board[i]);
	}
	free(board);

}

int updateGameStatus(char** board) {
//scan board for locations of mouse,cat,cheese
//update variables accordingly and return 1 if mouse wins, 2 if cat wins
	int i = 0, j = 0;

	char temp;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			temp = board[i][j];
			switch (temp) {
			case 'M':
				mouseRow = i;
				mouseCol = j;
				break;
			case 'C':
				catRow = i;
				catCol = j;
				break;
			case 'P':
				cheeseX = i;
				cheeseY = j;
				break;
			}
		}
	}
	if ((abs(cheeseX - mouseRow) + abs(cheeseY - mouseCol)) < 2)
		return 1;
	if ((abs(catRow - mouseRow) + abs(catCol - mouseCol)) < 2)
		return 2;
	else
		return 0;
}

void printBoard(char** board) {
	int i = 0, j = 0;
	char temp;
	printf("=============\n\n");
	if (board == NULL)
		return;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			temp = board[i][j];
			if (temp == '#')
				temp = '-';
			printf("%c ", temp);
		}
		printf("\n");
	}
//printf ("end printboard");
}

char** loadGame(int gamenum) {
//loads the game in "world_gamenum.txt"
	char temp = '0' + gamenum;
//printf ("%c", temp);
	int i = 0;
	char filename[20] = "world_#.txt";
	filename[6] = temp;

	char** board = calloc(7, sizeof(char*));

	for (i = 0; i < 7; i++) {
		board[i] = calloc(7, sizeof(char));
	}

//puts (filename);

	FILE * map = fopen(filename, "r");
	if (map == NULL) {
		exit(0);
	}
	fscanf(map, "%d\n", &max_turns);
	fscanf(map, "%s\n", turn);

	for (i = 0; i < 7; i++) {
		fscanf(map, "%s\n", board[i]);
	}

//printBoard(board);
	fclose(map);

	return board;
}

int checkBoard(char** board) {
	int mouse_flag = 0, cat_flag = 0, cheese_flag = 0;
	int i = 0, j = 0;
	char temp;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			temp = board[j][j];
			if (temp == 'C') {
				if (cat_flag)
					return 0;
				else
					cat_flag = 1;
			}
			if (temp == 'M') {
				if (mouse_flag)
					return 0;
				else
					mouse_flag = 1;
			}
			if (temp == 'P') {
				if (cheese_flag)
					return 0;
				else
					cheese_flag = 1;
			}
		}
	}
	return 1;
}

int saveGame(char** board, int gamenum) {
	if (gamenum > 5 || gamenum < 0)
		exit(0);

//printf("%s", board[6]);
	int i = 0;
	char temp = '0' + gamenum;
	char filename[20] = "world_#.txt";
	filename[6] = temp;

	FILE * map = fopen(filename, "w");
	if (map == NULL) {
		exit(0);
	}

	fprintf(map, "%d\n", max_turns);
	fprintf(map, "%s\n", turn);
	for (i = 0; i < 7; i++) {
		fprintf(map, "%s\n", board[i]);
	}

	return fclose(map);
}
