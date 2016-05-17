extraWarnings=-Werror -Wno-unused-but-set-variable
all: 
	gcc -g -Wall testProg.c -o testProg -static $(extraWarnings)
	gcc -g -Wall main.c -o mintdb $(extraWarnings)

test:
	make all
	./mintdb ./testProg
