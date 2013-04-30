/*
 * engine.c
 * This file is part of GTKTacToe
 *
 * Copyright (C) 2013 - Mason Fabel
 *
 * GTKTacToe is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GTKTacToe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Dialogues; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include <stdlib.h>
#include <stdio.h>

#include "engine.h"

static const char turnChars[3] = {'x', 'o', ' '};

static int turn = 0;
static int board[3][3] = { {2, 2, 2}, {2, 2, 2}, {2, 2, 2} };

static int isTaken(int row, int col) {
	return board[row][col] != 2;
}

static int isX(int row, int col) {
	return board[row][col] == 0;
}

static int isO(int row, int col) {
	return board[row][col] == 1;
}

void initEngine(void) {
	/* Holds the current cell's position */
	int x, y;

	/* Reset turn counter */
	turn = 0;

	/* Reset board */
	for(x = 0; x < 3; x++) {
		for(y = 0; y < 3; y++) {
			board[x][y] = 2;
		}
	}

	return;
}

char checkTurn(void) {
	return turnChars[turn];
}

int selectSquare(int row, int col) {
	/* Holds if the move is valid */
	int result;

	if(!isTaken(row, col)) {
		/* Empty cell */

		/* Set the cell */
		board[row][col] = turn;

		/* Increment the turn (flip between 0 and 1) */
		turn = (++turn) % 2;

		/* Success! */
		result = 1;
	} else {
		/* Full cell */
		result = 0;
	}

	return result;
}

char checkForWin(void) {
	int x, y, i = 0;
	char win = ' ';

	/* Loop until a win is found or all rows and columns have been checked */
	while(i < 3 && win == ' ') {
		/* Check for rows */
		if((board[i][0] != 2) && (board[i][0] == board[i][1]) && (board[i][0] == board[i][2])) win = turnChars[board[i][0]];
		/* Check for columns */
		else if((board[0][i] != 2) && (board[0][i] == board[1][i]) && (board[0][i] == board[2][i])) win = turnChars[board[0][i]];

		/* Increment row/column counter */
		i++;
	}

	/* Check diagonals if necessary */
	if(win == ' ') {
		/* Top left to bottom right */
		if((board[0][0] != 2) && (board[0][0] == board[1][1]) && (board[0][0] == board[2][2])) win = turnChars[board[0][0]];
		/* Bottom left to top right */
		else if((board[2][0] != 2) && (board[2][0] == board[1][1]) && (board[2][0] == board [0][2])) win = turnChars[board[2][0]];
	}

	/* Check for tie if necessary */
	if(win == ' ') {
		for(x = 0; x < 3; x++) {
			for(y = 0; y < 3; y ++) {
				if(board[x][y] != 2) {
					continue;
				} else {
					return win;
				}
			}
		}

		win = 't';
	}

	return win;
}
