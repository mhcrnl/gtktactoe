CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -D__USE_FIXED_PROTOTYPES__ -DDBG
GTKFLAGS = -Wl,--export-dynamic `pkg-config --cflags gtk+-3.0 gmodule-export-2.0` `pkg-config --libs gtk+-3.0 gmodule-export-2.0`
OBJ = gtktactoe.o

all : gtktactoe

rebuild :
	make clean
	make all

gtktactoe : $(OBJ)
	$(CC) $(CFLAGS) -o gtktactoe $(OBJ) $(GTKFLAGS)

gtktactoe.o : gtktactoe.c
	$(CC) $(CFLAGS) -c gtktactoe.c $(GTKFLAGS)

.PHONY : install clean

install :

clean :
	-rm -f gtk_app $(OBJ)
