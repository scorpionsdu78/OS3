CFLAGS = -lpthread
CC = gcc

main: main.c
	$(CC) $^ -o $@ $(CFLAGS)
	$ > /tmp/sem-mutex-key
	$ > /tmp/sem-buffer-count-key
	$ > /tmp/sem-spool-signal-key

clean:
	rm -f main.o main

test:  main
	@echo -e "\nTEST\n"
	./main
all: main
	./main
