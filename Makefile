CFLAGS = -g -Wall -pedantic -ansi -Wno-unused-result -D _BSD_SOURCE -O2
CC = gcc

D_TESTS = ./tests
D_SRC = ./led_matrix_ino


$(D_SRC)/to-bin.out: $(D_SRC)/to-bin.c
	@$(CC) $(CFLAGS) -o $@ $^


.PHONY: clean

clean:
	find . -name "*.out" -delete
