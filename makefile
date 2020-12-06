default: replay

replay: src/replay.c src/invmenu.c src/shopmenu.c src/dungeon.c src/charactermake.c src/replay.h src/startmenu.c
	gcc -o replay src/replay.c src/invmenu.c src/shopmenu.c src/dungeon.c src/charactermake.c src/startmenu.c

clean:
	$(RM) replay

