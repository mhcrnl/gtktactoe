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
int COMPUTER = 0;
int NEWGAME = 0;

/* Button callback structures */
struct Cell {
	GtkButton *button;
	gulong handler;
	int index;
};

/* Signal handlers */
static void finish(int sig);
static void clickEvent(GtkWidget *emitter, struct Cell *cell);
static void newGameEvent(GtkWidget *emitter);
static void checkboxEvent(GtkWidget *emitter);

/* CLI messages */
static void displayHelp(char *name);
static void displayVersion(char *name);

/* Helpers */
static void displayVictor(GtkLabel *label);

int main(int argc, char **argv) {
	/* Declare internal variables */
	int i;
	int firstTurn = 1;
	int secondTurn = 0;
	int row, col, index;
	int windowWidth = 600;
	int windowHeight = 400;
	char labelText[50];

	/* Create GTK Objects */
	GtkWindow *window;
	GtkWidget *vBox;
	GtkWidget *menubar;
	GtkWidget *filemenu;
	GtkWidget *file;
	GtkWidget *newGameButton;
	GtkWidget *checkbox;
	GtkWidget *quitButton;
	GtkGrid *board;
	GtkLabel *label;
	GtkButton *button;
	struct Cell cells[9];

	/* Keyboard Shortcuts */
	GtkAccelGroup *accel_group;

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

		/* VBox */
		vBox = gtk_vbox_new(FALSE, 0);
		gtk_container_add(GTK_CONTAINER(window), vBox);

		/* Menubar */
		menubar = gtk_menu_bar_new();
		filemenu = gtk_menu_new();

		accel_group = gtk_accel_group_new();
		gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

		file = gtk_menu_item_new_with_mnemonic("_Game");
		newGameButton = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, accel_group);
		checkbox = gtk_check_menu_item_new_with_label("Computer Game");
		quitButton = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);

		gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
		gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), newGameButton);
		gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), checkbox);
		gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), gtk_separator_menu_item_new());
		gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quitButton);
		gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
		gtk_box_pack_start(GTK_BOX(vBox), menubar, FALSE, FALSE, 0);

		g_signal_connect(newGameButton, "activate", G_CALLBACK(newGameEvent), 0);
		g_signal_connect(checkbox, "activate", G_CALLBACK(checkboxEvent), 0);
		g_signal_connect(quitButton, "activate", G_CALLBACK(finish), 0);

		/* TicTacToe Grid */
		board = gtk_grid_new();
		gtk_grid_set_row_homogeneous(board, gtk_true());
		gtk_grid_set_column_homogeneous(board, gtk_true());
		gtk_container_add(GTK_CONTAINER(vBox), board);

		/* TicTacToe Turn Indicator */
		label = gtk_label_new(NULL);
		gtk_grid_attach(board, label, 0, 0, 3, 1);

		/* TicTacToe Cells */
		for(i = 0; i < 9; i++) {
			cells[i].button = gtk_button_new();
			cells[i].index = i;
			cells[i].handler = g_signal_connect(cells[i].button, "clicked", G_CALLBACK(clickEvent), &cells[i]);
			sprintf(labelText, "%s/share/gtktactoe/sprites/empty.png", PATH);
			gtk_button_set_image(cells[i].button, gtk_image_new_from_file(labelText));
			gtk_grid_attach(board, cells[i].button, i % 3, (i / 3) + 1, 1, 1);
		}

		gtk_widget_show_all(window);

		if(VERBOSE) printf("DONE\n");

		/* Initialize the game engine */
		if(VERBOSE) printf("Initializing Engine...");
		initEngine();

		/* Show the window */
		if(VERBOSE) printf("DONE\nLaunching %s\n", argv[0]);
		gtk_widget_show(window);

		/* Main loop */
		while(1) {
			if(COMPUTER) {
				while(checkForWin() == ' ' && !NEWGAME) {
					gtk_main_iteration();
					sprintf(labelText, "%c's turn", toupper(checkTurn()));
					gtk_label_set_text(label, labelText);
					sprintf(labelText, "%s/share/gtktactoe/sprites/O.png", PATH);

					if(checkTurn() == 'o' && checkForWin() == ' ') {
						index = getBestIndex();

						/* It turns out that it is good to be specific on the first and second turns */
						if(firstTurn) {
							row = 1;
							col = 1;

							/* Try to get the middle square. */
							if(!selectSquare(row, col)) {
								row = 2;
								col = 2;
								selectSquare(row, col);
							}

							button = cells[rowColToIndex(row, col)].button;

							gtk_button_set_image(button, gtk_image_new_from_file(labelText));
							gtk_widget_set_sensitive(button, FALSE);

							firstTurn = 0;
							secondTurn = 1;

							continue;
						}

						if(secondTurn) {
							secondTurn = 0;

							if(isO(1, 1) && index == -1) {
								if(!isTaken(1, 0) && !isTaken(1, 2)) {
									row = 1;
									col = 0;
									selectSquare(row, col);
								} else {
									row = 0;
									col = 1;
									selectSquare(row, col);
								}

								button = cells[rowColToIndex(row, col)].button;
								gtk_button_set_image(button, gtk_image_new_from_file(labelText));
								gtk_widget_set_sensitive(button, FALSE);

								continue;
							}
						}

						/* Randomly select a square if there is nothing that *should* be done */
						if(index == -1) {
							do {
								row = rand() % 3;
								col = rand() % 3;

								if(!isTaken(row, col)) index = rowColToIndex(row, col);
							} while(index == -1);
						}

						row = index / 3;
						col = index % 3;

						selectSquare(row, col);

						button = cells[index].button;

						gtk_button_set_image(button, gtk_image_new_from_file(labelText));
						gtk_widget_set_sensitive(button, FALSE);
					}
				}

				if(!NEWGAME) {
					/* Display the victor */
					displayVictor(label);

					/* Turn off the sensitivity of the cells */
					for(i = 0; i < 9; i++) gtk_widget_set_sensitive(cells[i].button, FALSE);

					/* Wait for a new game */
					while(!NEWGAME) gtk_main_iteration();
				}

				/* Reset the images */
				for(i = 0; i < 9; i++) {
					sprintf(labelText, "%s/share/gtktactoe/sprites/empty.png", PATH);
					gtk_button_set_image(cells[i].button, gtk_image_new_from_file(labelText));
				}

				/* Make the cells sensitive again */
				for(i = 0; i < 9; i++) gtk_widget_set_sensitive(cells[i].button, TRUE);

				/* Reset */
				NEWGAME = 0;
				firstTurn = 1;

				initEngine();

				continue;
			}

			while(checkForWin() == ' ' && !NEWGAME) {
				gtk_main_iteration();
				sprintf(labelText, "%c's turn", toupper(checkTurn()));
				gtk_label_set_text(label, labelText);
			}

			if(!NEWGAME) {
				/* Display the victor */
				displayVictor(label);

				/* Turn off the sensitivity of the cells */
				for(i = 0; i < 9; i++) gtk_widget_set_sensitive(cells[i].button, FALSE);

				/* Wait for NEWGAME to change */
				while(!NEWGAME) gtk_main_iteration();
			}

			/* Reset the images */
			for(i = 0; i < 9; i++) {
				sprintf(labelText, "%s/share/gtktactoe/sprites/empty.png", PATH);
				gtk_button_set_image(cells[i].button, gtk_image_new_from_file(labelText));
			}

			/* Make the cells sensitive again */
			for(i = 0; i < 9; i++) gtk_widget_set_sensitive(cells[i].button, TRUE);

			/* Reset */
			NEWGAME = 0;

			initEngine();
		}
	} else {
		/* GTK failed to load */
		if(VERBOSE) printf("FAILED!\n");
		fprintf(stderr, "Unable to load GTK\n");
		finish(1);
	}

	return 0;
}

static void displayVictor(GtkLabel *label) {
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

static void clickEvent(GtkWidget *emitter, struct Cell *cell) {
	char player;
	char filename[50];
	int index, row, col;
	GtkButton *button;

	/* We don't want the player to accidentally play on the computer's turn */
	if(COMPUTER) if(checkTurn == 'o') return;

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
		gtk_widget_set_sensitive(button, FALSE);
	}

	return;
}

static void newGameEvent(GtkWidget *emitter) {
	NEWGAME = 1;

	return;
}

static void checkboxEvent(GtkWidget *emitter) {
	/* Flip between 0 and 1 */
	COMPUTER = (++COMPUTER) % 2;

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
	if(VERBOSE && sig == 0) printf("Closing GTK...DONE\n");
	if(DEBUG) fprintf(stdout, "SIGNAL: %d\n", sig);
	exit(0);
}
