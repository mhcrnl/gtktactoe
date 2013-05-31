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
#include <string.h>
#include <ctype.h>

#include "engine.h"
#include "libgtktactoe.h"

/* Set version information */
const int VERSION = 0;
const int MAJ_REV = 1;
const int MIN_REV = 2;

/* Set application flags */
int VERBOSE = 0;
int DEBUG = 0;

/* Global Variables */
int CHECKBOX = 0;
int COMPUTER = 0;
int NEWGAME = 0;

int main(int argc, char **argv) {
	/* Declare internal variables */
	int i, index;
	int thereIsAGameInProgress = 1;
	/* int windowWidth = 600;
	int windowHeight = 400; */
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
		window = (GtkWindow *) gtk_window_new(GTK_WINDOW_TOPLEVEL);
		g_signal_connect(window, "delete-event", G_CALLBACK(finish), 0);

		/* VBox */
		vBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
		gtk_container_add((GtkContainer *) window, vBox);

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
		board = (GtkGrid *) gtk_grid_new();
		gtk_grid_set_row_homogeneous(board, gtk_true());
		gtk_grid_set_column_homogeneous(board, gtk_true());
		gtk_container_add(GTK_CONTAINER(vBox), (GtkWidget *) board);

		/* TicTacToe Turn Indicator */
		label = (GtkLabel *) gtk_label_new(NULL);
		gtk_grid_attach(board, (GtkWidget *) label, 0, 0, 3, 1);

		/* TicTacToe Cells */
		for(i = 0; i < 9; i++) {
			cells[i].button = (GtkButton *) gtk_button_new();
			cells[i].index = i;
			cells[i].handler = g_signal_connect(cells[i].button, "clicked", G_CALLBACK(clickEvent), &cells[i]);
			sprintf(labelText, "%s/share/gtktactoe/sprites/empty.png", PATH);
			gtk_button_set_image(cells[i].button, gtk_image_new_from_file(labelText));
			gtk_grid_attach(board, (GtkWidget *) cells[i].button, i % 3, (i / 3) + 1, 1, 1);
		}

		gtk_widget_show_all((GtkWidget *) window);

		if(VERBOSE) printf("DONE\n");

		/* Initialize the game engine */
		if(VERBOSE) printf("Initializing Engine...");
		initEngine();

		/* Show the window */
		if(VERBOSE) printf("DONE\nLaunching %s\n", argv[0]);
		gtk_widget_show((GtkWidget *) window);

		/* Main loop */
		while(1) {
			gtk_main_iteration();
			sprintf(labelText, "%c's turn", toupper(checkTurn()));
			gtk_label_set_text(label, labelText);

			/* If the newGame event has been called, then we need to start a new game */
			if(NEWGAME) thereIsAGameInProgress = 0;

			/* Gameplay */
			if(thereIsAGameInProgress) {
				if(checkForWin() == ' ' && checkTurn() == 'o' && COMPUTER) {	
					sprintf(labelText, "%s/share/gtktactoe/sprites/O.png", PATH);

					index = getBestIndex();

					selectSquare((index / 3), (index % 3));

					button = cells[index].button;

					gtk_button_set_image(button, gtk_image_new_from_file(labelText));
					gtk_widget_set_sensitive((GtkWidget *) button, FALSE);

				} else if(checkForWin() == ' ' && thereIsAGameInProgress) continue;
				/* if checkForWin() != ' ', then the game has ended */
				else thereIsAGameInProgress = 0;
			} else {
				/* If the game ended without the newGame event being called... */
				if(NEWGAME == 0) {
					/* Display the victor */
					displayVictor(label);

					/* Turn off the sensitivity of the cells */
					for(i = 0; i < 9; i++) gtk_widget_set_sensitive((GtkWidget *) cells[i].button, FALSE);

					/* Wait for the newGame event to be called */
					while(NEWGAME == 0) gtk_main_iteration();
				}

				/* Reset the images */
				for(i = 0; i < 9; i++) {
					sprintf(labelText, "%s/share/gtktactoe/sprites/empty.png", PATH);
					gtk_button_set_image(cells[i].button, gtk_image_new_from_file(labelText));
				}

				/* Make the cells sensitive again */
				for(i = 0; i < 9; i++) gtk_widget_set_sensitive((GtkWidget *) cells[i].button, TRUE);

				/* Reset */
				if(CHECKBOX) COMPUTER = 1;
				else COMPUTER = 0;

				NEWGAME = 0;
				thereIsAGameInProgress = 1;

				initEngine();
			}
		}
	} else {
		/* GTK failed to load */
		if(VERBOSE) printf("FAILED!\n");
		fprintf(stderr, "Unable to load GTK\n");
		finish(1);
	}

	return 0;
}
