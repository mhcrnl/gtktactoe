CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -D__USE_FIXED_PROTOTYPES__ -ggdb3 -DSIM
GTKFLAGS = `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

OBJ = main.o

all : gtk_app

gtk_app : $(OBJ)
	$(CC) $(CFLAGS) $(GTKFLAGS) -o gtk_app $(OBJ)

.PHONY : install clean

install :

clean :
	-rm -f gtk_app $(OBJ)
