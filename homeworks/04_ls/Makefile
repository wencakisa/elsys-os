all: main

helpers.o: helpers.h helpers.c

ls.o: ls.h ls.c

main.o: main.c

main: helpers.o ls.o main.o
	gcc -o ls -Wall -pedantic helpers.o ls.o main.o

clean:
	rm -f *.o ls
