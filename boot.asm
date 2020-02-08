; simple boot sector to load next sectors of floppy disk.
; by Philip R. Simonson
; ===============================================================
[bits 16]
[org 0x7c00]
[section .text]
global _start
jmp short _start

; BPB here

_start:
	mov byte [drive], dl
	xor ax, ax
	mov ds, ax
	mov es, ax
	cli
	mov ss, ax
	mov sp, 0xffff
	sti

	mov si, loading
	call print

	; load some sectors from disk
	mov bx, 0x0100
	mov es, bx
	xor bx, bx
	mov al, 4
	call read_sector
	jmp 0x0100:0x0000

	mov si, reboot
	call print
	int 19h

getc:
	xor ax, ax
	int 16h
	ret

print:
	lodsb
	cmp al, 24h
	je .done
	mov ah, 0eh
	mov bx, 0007h
	mov cx, 0001h
	int 10h
	jmp short print
.done:
	ret

read_sector:
	mov ah, 02h
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov dl, byte [drive]
	int 13h
	jc .retry
	mov si, done
	call print
	ret
.retry:
	xor ax, ax
	int 13h
	jc .retry
	mov si, error
	call print
	jmp short read_sector

; data
loading db "Loading system...",0ah,0dh,24h
done db "Done loading!",0ah,0dh,24h
reboot db "Rebooting...",0ah,0dh,24h
error db "Disk read error.",0ah,0dh,24h
crlf db 0ah,0dh,24h
drive db 0

times 510-($-$$) db 0
dw 0xaa55
