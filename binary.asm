; binary.asm
; by 5n4k3
[bits 16]
[org 0h]
[section .text]
global _start

_start:
	xor ax, ax
	mov ds, ax
	mov es, ax

	mov dx, message
	call print
	xor ax, ax
	int 16h

	; reset segments and jump back to IO.SYS
	pop ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	jmp 0x0100:0x0000

print:
	mov si, dx
.loop:
	lodsb
	cmp al, 24h
	je .done
	mov ah, 0eh
	mov bx, 7
	mov cx, 1
	int 10h
	jmp short .loop
.done:
	ret

; data
message db "Hello world from somewhere else.",0ah,0dh,24h

