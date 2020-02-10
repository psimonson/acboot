; simple boot sector to load next sectors of floppy disk.
; by Philip R. Simonson
; ===============================================================
[bits 16]
[org 0x7c00]
[section .text]
global _start
jmp short _start
nop

; BPB here

_start:
	mov byte [drive], dl
	xor ax, ax
	mov ds, ax
	mov es, ax
	cli
	mov bx, 0x8000
	mov ss, bx
	mov sp, ax
	sti
	cld

	mov si, loading
	call print

	; load some sectors from disk
	mov bx, word [root]
	mov byte [sector], 1
	mov byte [blocks], 1
	call read_sectors

	; load kernel
	mov word [kernel], 0
	call load_kernel
	jmp 0x0000:0x0100
	call reboot

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

load_kernel:
	mov bx, word [root]
.next:
	mov al, byte [bx]
	cmp al, 2
	jne .no_match
.match:
	xor cx, cx
	mov ch, byte [bx+1]
	mov byte [blocks], ch
	mov cl, byte [bx+2]
	mov byte [sector], cl
	mov bx, 0x0100
	mov ax, word [kernel]
.loop:
	call read_sectors
	inc byte [sector]
	mov al, byte [blocks]
	cmp byte [sector], al
	jl .loop
	mov si, done
	call print
	ret
.no_match:
	add bx, 16
	jmp short .next

read_sectors:
.main:
	mov di, 5
.loop:
	push ax
	push bx
	push cx
	mov ah, 2
	mov al, 1
	mov ch, 0
	mov cl, byte [sector]
	mov dh, 0
	mov dl, byte [drive]
	int 13h
	jnc .success
	; read failed, decrement retry count.
	xor ax, ax
	int 13h
	dec di
	pop cx
	pop bx
	pop ax
	jnz .loop
	mov si, fail
	call print
	call reboot
.success:
	mov si, progress
	call print
	pop cx
	pop bx
	pop ax
	add bx, 0x200
	inc ax
	loop .main
	ret

reboot:
	mov si, reboot_msg
	call print
	call getc
	xor ax, ax
	int 19h

; data
loading db "Loading system...",0ah,0dh,24h
done db 0ah,0dh,"Done loading!",0ah,0dh,24h
reboot_msg db "Press any key to try again...",0ah,0dh,24h
error db "Disk read error.",0ah,0dh,24h
crlf db 0ah,0dh,24h
drive db 0
root dw 0x1000
kernel dw 0x0000
sector db 0
blocks db 0
progress db 2eh,24h
fail db 0ah,0dh,"Disk read error :(",0ah,0dh,24h

times 510-($-$$) db 0
dw 0xaa55
