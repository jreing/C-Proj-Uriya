/*
 * logic.h
 *
 *  Created on: Apr 19, 2015
 *      Author: ub
 */

#ifndef LOGIC_H_
#define LOGIC_H_


#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

char** loadGame (int gamenum);
//void freeBoard(char** board);
int move (char*** board, int direction);
void freeBoard(char** board);
int updateGameStatus (char** board);
void printBoard(char** board);
char** loadGame (int gamenum);

char turn[7];//toggle cat's turn.
#endif /* LOGIC_H_ */
