/*
 * libgtktactoe.h
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

#ifndef _LIBGTKTACTOE_H_
#define _LIBGTKTACTOE_H_

struct Cell {
	GtkButton *button;
	gulong handler;
	int index;
};

void finish(int sig);
void clickEvent(GtkWidget *emitter, struct Cell *cell);
void newGameEvent(GtkWidget *emitter);
void checkboxEvent(GtkWidget *emitter);
void displayHelp(char *name);
void displayVersion(char *name);
void displayVictor(GtkLabel *label);

#endif /* _LIBGTKTACTOE_H_ */
