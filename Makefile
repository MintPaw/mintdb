all: 
	gcc -g -Wall testProg.c -o testProg -static
	gcc -g -Wall main.c -o mintdb

test:
	make all
	./mintdb ./testProg
