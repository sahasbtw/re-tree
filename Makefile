CFLAGS = -Wall -Wextra -Werror --std=c99
CC = gcc

re-tree: main.c
	$(CC) $(CFLAGS) main.c -o re-tree
