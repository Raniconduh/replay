INCLUDEDIR = include
CFLAGS = $(shell xml2-config --cflags) -Wall -Wextra -I$(INCLUDEDIR)
LIBS = $(shell xml2-config --libs)
SOURCES = src/*.c

default: replay

replay: $(SOURCES)
	gcc -o replay $(CFLAGS) $(SOURCES) $(LIBS)

debug: $(SOURCES)
	gcc -o replay $(CFLAGS) -g -DDEBUG $(SOURCES) $(LIBS)

clean:
	$(RM) replay

