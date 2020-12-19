CFLAGS = $(shell xml2-config --cflags)
LIBS = $(shell xml2-config --libs)

default: replay

replay: src/replay.c src/invmenu.c src/shopmenu.c src/dungeon.c src/charactermake.c src/replay.h src/startmenu.c
	gcc -o replay $(CFLAGS) -Wall -Wextra src/startmenu.c src/replay.c src/invmenu.c src/shopmenu.c src/dungeon.c src/charactermake.c $(LIBS)

clean:
	$(RM) replay

