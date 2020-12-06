default: replay

replay: src/replay.c src/invmenu.c src/shopmenu.c src/dungeon.c src/charactermake.c src/replay.h
	gcc -o replay src/replay.c src/invmenu.c src/shopmenu.c src/dungeon.c src/charactermake.c

clean:
	$(RM) replay

