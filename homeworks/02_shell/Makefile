all: shell

helpers.o: helpers.c

parser.o: parser.c

interpreter.o: interpreter.c

main.o: main.c

shell: main.o helpers.o parser.o interpreter.o
	gcc -o shell main.o helpers.o parser.o interpreter.o
	zip -r 02_shell.zip ./

clean:
	rm -rf *.o shell 02_shell.zip
