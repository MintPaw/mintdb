all: 
	gcc -g -Wall main.c -o mintdb

testLs:
	make all
	./mintdb ls -l
