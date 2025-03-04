
CC = gcc
CFLAGS = -g -Iinclude
LDFLAGS = -lncurses

src = $(wildcard *.c */*.c) src/display/display_ncurses.c
obj = ${src:.c=.o}


all: pikno

pikno: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(obj)

fclean: clean
	rm -f pikno

re: fclean all

.PHONY: all clean fclean re
