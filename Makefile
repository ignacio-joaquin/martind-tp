CC=gcc
CFLAGS=-g
TARGET=juego

all:
	$(CC) $(CFLAGS) -o $(TARGET) src/main.c src/bot.c src/juego.c src/struct_bot.c

clean:
	rm -f $(TARGET)
