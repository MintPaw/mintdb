extraWarnings=-Werror -Wno-unused-but-set-variable -Wno-unused-variable
all: 
	gcc -g -Wall src/main.c -o bin/mintdb $(extraWarnings)
	gcc -g -Wall src/testProg.c -o bin/testProg -static $(extraWarnings)
	nasm -f elf64 src/testProgSimple.asm -l obj/testProgSimple.lst -o obj/testProgSimple.o
	ld obj/testProgSimple.o -o bin/testProgSimple

test:
	make all
	bin/mintdb bin/testProg

testSimple:
	make all
	bin/mintdb bin/testProgSimple
