SECTION .data
	msg:	db "Hello World",10	; the string to print, 10=cr
	len:	equ $-msg		; "$" means "here"
	; len is a value, not an address

SECTION .text
	global _start		; make label available to linker 

_start:
	mov	edx,len		; arg3, length of string to print
	mov	ecx,msg		; arg2, pointer to string
	mov	ebx,1		; arg1, where to write, screen
	mov	eax,4		; write sysout command to int 80 hex
	int	0x80		; interrupt 80 hex, call kernel

	mov	eax,1		; exit command to kernel (did not set normal exit)
	int	0x80		; interrupt 80 hex, call kernel
