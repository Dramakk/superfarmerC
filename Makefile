CC=gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
LFLAGS = -lncurses
NAME = superfarmer
SRC = main.c wymiana.c kostka.c
DEPS = kostka.h wymiana.h
OBJS = main.o kostka.o wymiana.o
YOU: $(SRC) $(NAME)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJS) $(NAME)
