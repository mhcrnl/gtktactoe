CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -D__USE_FIXED_PROTOTYPES__ -DDBG
GTKFLAGS = -Wl,--export-dynamic `pkg-config --cflags gtk+-3.0 gmodule-export-2.0` `pkg-config --libs gtk+-3.0 gmodule-export-2.0`
OBJ = main.o

all : gtk_app

rebuild :
	make clean
	make all

gtk_app : $(OBJ)
	$(CC) $(CFLAGS) -o gtk_app $(OBJ) $(GTKFLAGS)

main.o : main.c
	$(CC) $(CFLAGS) -c -o main.o main.c $(GTKFLAGS)

.PHONY : install clean

install :

clean :
	-rm -f gtk_app $(OBJ)
