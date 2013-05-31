/*
 * libgtktactoe.c
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

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

#include "libgtktactoe.h"
#include "engine.h"

/* External Variables */
extern const int VERSION;
extern const int MAJ_REV;
extern const int MIN_REV;
extern int VERBOSE;
extern int DEBUG;
extern int CHECKBOX;
extern int COMPUTER;
extern int NEWGAME;

void displayVictor(GtkLabel *label) {
	char victor = checkForWin();
	char labelText[8];

	if(victor == 't') {
		if(VERBOSE) printf("Tie!");
		gtk_label_set_text(label, "Tie!");
	} else {
		if(VERBOSE) printf("%c's won!\n", toupper(victor));
		sprintf(labelText, "%c's won!", toupper(victor));
		gtk_label_set_text(label, labelText);
	}

	return;
}

void clickEvent(GtkWidget *emitter, struct Cell *cell) {
	char player;
	char filename[50];
	int index, row, col;
	GtkButton *button;

	/* We don't want the player to accidentally play on the computer's turn */
	if(COMPUTER) if(checkTurn() == 'o') return;

	player = toupper(checkTurn());
	index = cell->index;
	button = cell->button;
	row = index / 3;
	col = index % 3;

	if(DEBUG) printf("Button %d was pressed\n", index);

	if(selectSquare(row, col)) {
		if(VERBOSE) printf("%c claims row %d, column %d\n", player, row, col);
		if(VERBOSE) printf("It is now %c's turn\n", toupper(checkTurn()));

		sprintf(filename, "%s/share/gtktactoe/sprites/%c.png", PATH, player);
		if(DEBUG) printf("Setting button %d's image to %s\n", index, filename);
		gtk_button_set_image(button, gtk_image_new_from_file(filename));
		gtk_widget_set_sensitive((GtkWidget *) button, FALSE);
	}

	return;
}

void newGameEvent(GtkWidget *emitter) {
	NEWGAME = 1;

	return;
}

void checkboxEvent(GtkWidget *emitter) {
	/* Flip between 0 and 1 */
	CHECKBOX = (++CHECKBOX) % 2;

	return;
}

void displayHelp(char *name) {
	printf("Usage: %s [-w] [-h] [OPTIONS]...\n", name);
	printf("Plays %s, a GTK+ TicTacToe game\n", name);
	printf("\n  -?. --help\t\tdisplay this help and exit\n");
	printf("  -V, --version\t\toutput version information and exit\n");
	printf("  -v, --verbose\t\trun with maximum verbosity\n");
	printf("  -D, --debug\t\trun in debugging mode\n");

	return;
}

void displayVersion(char *name) {
	printf("%s %d.%d.%d\n\n", name, VERSION, MAJ_REV, MIN_REV);
	printf("Copyright (C) 2013 Mason Fabel\n");
	printf("License GPLv2+: GNU GPL version 2 or latter <http://gnu.org/licenses/glp/html>\n");
	printf("This is free software: you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n\n");
	printf("Written by Mason Fabel\n");

	return;
}

void finish(int sig) {
	if(VERBOSE && sig == 0) printf("Closing GTK...DONE\n");
	if(DEBUG) fprintf(stdout, "SIGNAL: %d\n", sig);
	exit(0);
}
