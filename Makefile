CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -D__USE_FIXED_PROTOTYPES__ -ggdb3
GTKFLAGS = -Wl,--export-dynamic `pkg-config --cflags gtk+-3.0 gmodule-export-2.0` `pkg-config --libs gtk+-3.0 gmodule-export-2.0`
OBJ = gtktactoe.o libgtktactoe.o engine.o
PREFIX_QUOTED = '"/usr"'
PREFIX = /usr

.PHONY : all rebuild install clean

all : gtktactoe

rebuild :
	make clean
	make all

gtktactoe : $(OBJ)
	$(CC) $(CFLAGS) -o gtktactoe $(OBJ) $(GTKFLAGS)

gtktactoe.o : gtktactoe.c libgtktactoe.h engine.h
	$(CC) $(CFLAGS) -DPATH=$(PREFIX_QUOTED) -c gtktactoe.c $(GTKFLAGS)

engine.o : engine.c engine.h

libgtktactoe.o : libgtktactoe.c libgtktactoe.h engine.h
	$(CC) -DPATH=$(PREFIX_QUOTED) -c libgtktactoe.c $(GTKFLAGS)

install : gtktactoe
	test -d $(PREFIX)/bin || mkdir -p $(PREFIX)/bin
	test -d $(PREFIX)/share/gtktactoe/sprites || mkdir -p $(PREFIX)/share/gtktactoe/sprites

	install -o root -g games -m 0755 ./gtktactoe $(PREFIX)/bin
	install -o root -g games -m 0644 ./sprites/*.png $(PREFIX)/share/gtktactoe/sprites

clean :
	rm -f gtktactoe $(OBJ)
