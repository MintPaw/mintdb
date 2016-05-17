all: 
	gcc -g -Wall main.c -o mintdb

testEcho:
	make all
	./mintdb echo hi

testLs:
	make all
	./mintdb ls -l
