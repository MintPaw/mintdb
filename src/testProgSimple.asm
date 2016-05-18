SECTION .data
	msg1:	db "Hello",0xa
	len1:	equ $-msg1
	msg2: db "world!", 0xa
	len2: equ $-msg2

SECTION .text
	global _start		; make label available to linker 

_start:
	mov	edx,len1		; arg3, length of string to print
	mov	ecx,msg1		; arg2, pointer to string
	mov	ebx,1		; arg1, where to write, screen
	mov	eax,4		; write sysout command to int 80 hex
	int	0x80		; interrupt 80 hex, call kernel

	mov	edx,len2		; arg3, length of string to print
	mov	ecx,msg2		; arg2, pointer to string
	mov	ebx,1		; arg1, where to write, screen
	mov	eax,4		; write sysout command to int 80 hex
	int	0x80		; interrupt 80 hex, call kernel

	mov	eax,1		; exit command to kernel (did not set normal exit)
	int	0x80		; interrupt 80 hex, call kernel
