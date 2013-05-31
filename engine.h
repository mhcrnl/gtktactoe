/*
 * engine.h
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
 * along with GTKTacToe; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

void initEngine(void);
char checkForWin(void);
char checkTurn(void);
int selectSquare(int row, int col);
int getBestIndex(void);

#endif /* _ENGINE_H_ */
