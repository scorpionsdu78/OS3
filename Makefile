PROG=main
LDLIBS=-lrt 
CFLAGS2=-Wall -Werror -DPOSIX
CFLAGS=-Wall -Werror 

${PROG}: main.o
	gcc ${CFLAGS} -o main main.o ${LDLIBS}

main.o: main.c
	gcc ${CFLAGS} -c main.c

test: ${PROG}
	@echo -e "\n\nTESTING\n\n"
	@./${PROG}

clean:
	rm ${PROG}

.PHONY: clean test
