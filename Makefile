extraWarnings=-Werror -Wno-unused-but-set-variable
all: 
	gcc -g -Wall testProg.c -o testProg -static $(extraWarnings)
	gcc -g -Wall main.c -o mintdb $(extraWarnings)
	nasm -f elf64 testProgSimple.asm -l testProgSimple.lst 
	ld testProgSimple.o -o testProgSimple

test:
	make all
	./mintdb ./testProg

testSimple:
	make all
	./mintdb ./testProgSimple
