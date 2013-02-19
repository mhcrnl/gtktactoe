/*
 * main.c
 * This file is part of GTK-Test
 *
 * Copyright (C) 2013 - Mason Fabel
 *
 * GTK-Test is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GTK-Test is distributed in the hope that it will be useful,
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

/* Set DEBUG mode from compiler flags */
#ifdef DBG
static const int DEBUG = 1;
#else
static const int DEBUG = 0;
#endif

/* Set version information */
static const int VERSION = 0;
static const int MAJ_REV = 0;
static const int MIN_REV = 1;

/* Signal handler */
static void finish(int sig);

/* CLI messages */
static void displayHelp(char *name);
static void displayVersion(char *name);

int main(int argc, char **argv) {
	/* Declare internal variables */
	int i;
	int exitSignal;
	int windowWidth = 600;
	int windowHeight = 400;

	/* Holds the GTK elements */
	GtkWindow *window;
	GtkButton *button;

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
		if(!(strcmp(argv[i], "-v") && strcmp(argv[i], "--version"))) {
			displayVersion(argv[0]);
			finish(0);
		}

		/* Check for application arguments */
		if(!(strcmp(argv[i], "-w") && strcmp(argv[i], "--width"))) {
			i++;
			if(DEBUG) printf("Setting width to %s\n", argv[i]);
			windowWidth = sscanf("%d", argv[i]);
		}
		if(!(strcmp(argv[i], "-h") && strcmp(argv[i], "--height"))) {
			i++;
			if(DEBUG) printf("Setting height to %s\n", argv[i]);
			windowHeight = sscanf("%d", argv[i]);
		}
	}

	/* Attempt to start GTK */
	if(gtk_init_check(&argc, &argv)) {
		/* GTK is good! */

		/* Create the main window */
		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
		gtk_window_resize(window, (gint) windowWidth, (gint) windowHeight);

		/* Create the button */
		button = gtk_button_new_with_label("Hello World!");
		gtk_widget_set_valign(button, GTK_ALIGN_START);
		gtk_widget_set_halign(button, GTK_ALIGN_START);
		gtk_container_add(GTK_CONTAINER(window), button);

		/* Show the window */
		gtk_widget_show_all(window);

		/* Give program control to GTK */
		gtk_main();

		/* GTK main loop is done */
		exitSignal = 0;
	} else {
		/* GTK failed to load */
		fprintf(stderr, "Unable to load GTK\n");
		exitSignal = 1;
	}

	/* End the program */
	finish(exitSignal);
	return 0;
}

static void displayHelp(char *name) {
	printf("Usage: %s [-w] [-h] [OPTIONS]...\n", name);
	printf("Plays %s, a GTK+ TicTacToe game\n", name);
	printf("\n  -?. --help\t\tdisplay this help and exit\n");
	printf("  -v, --version\t\toutput version information and exit");
	printf("  -h, --height\t\tset the window height\n");
	printf("  -w, --width\t\tset the window width\n");

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
	if(DEBUG) {
		fprintf(stdout, "SIGNAL: %d\n", sig);
	}
	exit(0);
}
