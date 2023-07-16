CC=gcc
CFLAGS=-lpthread
SOURCES=game.c

all: game run

game: $(SOURCES)
	$(CC) -o game $(SOURCES) $(CFLAGS)

run:
	./game