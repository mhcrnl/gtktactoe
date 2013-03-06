/*
 * gtktactoe.c
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

#include "engine.h"

/* Set version information */
static const int VERSION = 0;
static const int MAJ_REV = 1;
static const int MIN_REV = 2;

/* Set application flags */
int VERBOSE = 0;
int DEBUG = 0;

/* Button callback structures */
struct Cell {
	GtkButton *button;
	gulong handler;
	int index;
};

/* Signal handlers */
static void finish(int sig);
static void clickEvent(GtkWidget *emitter, struct Cell *cell);

/* CLI messages */
static void displayHelp(char *name);
static void displayVersion(char *name);

int main(int argc, char **argv) {
	/* Declare internal variables */
	int i;
	int exitSignal;
	int windowWidth = 600;
	int windowHeight = 400;
	char labelText[50];

	/* Create GTK Objects */
	GtkWindow *window;
	GtkGrid *board;
	GtkLabel *label;
	struct Cell cells[9];

	/* Initialize the signal handler function */
	(void) signal(SIGINT, finish);

	/* Parse arguments */
	for(i = 0; i < argc; i++) {
		if(DEBUG) printf("Argument %d: %s\n", i, argv[i]);

		/* Check for informational arguments */
		if(!(strcmp(argv[i], "-?") && strcmp(argv[i], "--help"))) {
			displayHelp(argv[0]);
			finish(0);
		}
		if(!(strcmp(argv[i], "-V") && strcmp(argv[i], "--version"))) {
			displayVersion(argv[0]);
			finish(0);
		}

		/* Check for application arguments */
		if(!(strcmp(argv[i], "-v") && strcmp(argv[i], "--verbose"))) {
			if(DEBUG) printf("Maximum verbosity\n");
			VERBOSE = 1;
		}
		if(!(strcmp(argv[i], "-D") && strcmp(argv[i], "--debug"))) {
			printf("Entering debugging mode...\n");
			DEBUG = 1;
			/* Include the VERBOSE flag with the DEBUG flag */
			VERBOSE = 1;
		}

		/* --- NOT IMPLEMENTED
		if(!(strcmp(argv[i], "-w") && strcmp(argv[i], "--width"))) {
			i++;
			if(DEBUG) printf("Setting width to %s\n", argv[i]);
			windowWidth = atoi(argv[i]);
		}
		if(!(strcmp(argv[i], "-h") && strcmp(argv[i], "--height"))) {
			i++;
			if(DEBUG) printf("Setting height to %s\n", argv[i]);
			windowHeight = atoi(argv[i]);
		}
		--- */
	}

	if(VERBOSE) printf("Loading GTK...");

	/* Attempt to start GTK */
	if(gtk_init_check(&argc, &argv)) {
		/* GTK is good! */
		if(VERBOSE) printf("DONE\n");

		/* Build GTK Layout */
		if(VERBOSE) printf("Building Interface...");

			/* Main Window */
			window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
			g_signal_connect(window, "delete-event", G_CALLBACK(finish), 0);

			/* TicTacToe Grid */
			board = gtk_grid_new();
			gtk_grid_set_row_homogeneous(board, gtk_true());
			gtk_grid_set_column_homogeneous(board, gtk_true());
			gtk_container_add(GTK_CONTAINER(window), board);
			gtk_widget_show(board);

			/* TicTacToe Turn Indicator */
			label = gtk_label_new(NULL);
			gtk_grid_attach(board, label, 0, 0, 3, 1);
			gtk_widget_show(label);

			/* TicTacToe Cells */
			for(i = 0; i < 9; i++) {
				cells[i].button = gtk_button_new();
				cells[i].index = i;
				cells[i].handler = g_signal_connect(cells[i].button, "clicked", G_CALLBACK(clickEvent), &cells[i]);
				sprintf(labelText, "%s/share/gtktactoe/sprites/empty.png", PATH);
				gtk_button_set_image(cells[i].button, gtk_image_new_from_file(labelText));
				gtk_grid_attach(board, cells[i].button, i % 3, (i / 3) + 1, 1, 1);
				gtk_widget_show(cells[i].button);
			}

			if(VERBOSE) printf("DONE\n");

		/* Initialize the game engine */
		if(VERBOSE) printf("Initializing Engine...");
		initEngine();

		/* Show the window */
		if(VERBOSE) printf("DONE\nLaunching %s\n", argv[0]);
		gtk_widget_show(window);

		/* Main loop */
		while(checkForWin() == ' ') {
			gtk_main_iteration();
			sprintf(labelText, "%c's turn", toupper(checkTurn()));
			gtk_label_set_text(label, labelText);
		}

		/* Display the victor */
		if(VERBOSE) printf("%c's won!\n", toupper(checkForWin()));
		sprintf(labelText, "%c's won!", toupper(checkForWin()));
		gtk_label_set_text(label, labelText);

		/* Disconnect clickEvent handlers */
		for(i = 0; i < 9; i++) {
			g_signal_handler_disconnect(cells[i].button, cells[i].handler);
		}

		/* Loop forever */
		gtk_main();

		/* GTK main loop is done */
		exitSignal = 0;
		if(VERBOSE) printf("Closing GTK...DONE\n");
	} else {
		/* GTK failed to load */
		if(VERBOSE) printf("FAILED!\n");
		fprintf(stderr, "Unable to load GTK\n");
		exitSignal = 1;
	}

	/* End the program */
	finish(exitSignal);
	return 0;
}

static void clickEvent(GtkWidget *emitter, struct Cell *cell) {
	char player;
	char filename[50];
	int index, row, col;
	GtkButton *button;

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
	}

	return;
}

static void displayHelp(char *name) {
	printf("Usage: %s [-w] [-h] [OPTIONS]...\n", name);
	printf("Plays %s, a GTK+ TicTacToe game\n", name);
	printf("\n  -?. --help\t\tdisplay this help and exit\n");
	printf("  -V, --version\t\toutput version information and exit\n");
	printf("  -v, --verbose\t\trun with maximum verbosity\n");
	printf("  -D, --debug\t\trun in debugging mode\n");

	return;
}

static void displayVersion(char *name) {
	printf("%s %d.%d.%d\n\n", name, VERSION, MAJ_REV, MIN_REV);
	printf("Copyright (C) 2013 Mason Fabel\n");
	printf("License GPLv2+: GNU GPL version 2 or latter <http://gnu.org/licenses/glp/html>\n");
	printf("This is free software: you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n\n");
	printf("Written by Mason Fabel\n");

	return;
}

static void finish(int sig) {
	if(DEBUG) fprintf(stdout, "SIGNAL: %d\n", sig);
	exit(0);
}
