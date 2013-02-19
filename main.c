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

/* Signal handler */
static void finish(int sig);

static void swap(GtkButton *theButton);

int main(int argc, char **argv) {
	int exitSignal;
	gint windowWidth = 600;
	gint windowHeight = 400;

	/* Holds the GTK elements */
	GtkWindow *window;
	GtkButton *button;

	/* Initialize the signal handler function */
	(void) signal(SIGINT, finish);

	/* Attempt to start GTK */
	if(gtk_init_check(&argc, &argv)) {
		/* GTK is good! */

		/* Create the main window */
		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
		gtk_window_resize(window, windowWidth, windowHeight);

		/* Create the button */
		button = gtk_button_new_with_label("Hello World!");
		gtk_widget_set_valign(button, GTK_ALIGN_START);
		gtk_widget_set_halign(button, GTK_ALIGN_START);
		g_signal_connect(button, "clicked", G_CALLBACK(swap), NULL);
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

static void swap(GtkButton* theButton) {
	gtk_button_set_label(theButton, "See ya later...");
}

static void finish(int sig) {
	if(DEBUG) {
		fprintf(stdout, "## SIGNAL: %d\n", sig);
	}
	exit(0);
}
