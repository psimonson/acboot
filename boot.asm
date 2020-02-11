; simple boot sector to load next sectors of floppy disk.
; by Philip R. Simonson
; ===============================================================
[bits 16]
[org 0x7c00]

jmp short _start
nop

SectorsPerTrack db 18
NumHeads db 2

_start:
	mov byte [drive], dl
	xor ax, ax
	mov ds, ax
	mov es, ax
	cli
	mov ss, ax
	mov sp, 0xffff
	sti

	; check if it's an 8086/80186/80286/80386
	mov cx, 0121h
	shl ch, cl
	jz .not_386 ; it's 8086
	push sp
	pop ax
	cmp ax, sp
	jne .not_386 ; it's 80186
	mov ax, 7000h
	push ax
	popf
	pushf
	pop ax
	and ax, 7000h
	cmp ax, 7000h
	je .system ; it's 386

.not_386:
	; cpu is 286 or lower
	mov si, not_supported
	call print
	call reboot

.system:
	mov si, loading
	call print

	; load some sectors from disk
	mov bx, word [root]
	mov ax, 1
	mov cx, 1
	call read_sectors

	; load kernel
	call load_kernel
	push 0x0100
	push 0x0000
	retf
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
	mov si, word [root]
.next:
	mov al, byte [si]
	cmp al, 3
	jne .no_match
.match:
	mov ax, word [si+4]
	mov cx, word [si+2]
	mov bx, 0x0100
	mov es, bx
	xor bx, bx
	call read_sectors
	mov si, done
	call print
	ret
.no_match:
	add si, 16
	jmp short .next

lbachs:
	; calculate sector
	push bx
	push ax
	xor dx, dx
	; sectors first
	div word [SectorsPerTrack]
	inc dl
	mov byte [absSector], dl
	xor dx, dx
	; now heads
	div word [NumHeads]
	mov byte [absHead], dl
	; finally cylinders
	mov ax, word [NumHeads]
	mul word [SectorsPerTrack]
	xor dx, dx
	xchg bx, ax
	pop ax
	div bx
	mov byte [absTrack], al
	xor dx, dx
	pop bx
	ret

read_sectors:
.main:
	mov di, 5
.loop:
	push ax
	push bx
	push cx
	call lbachs
	mov ah, 2
	mov al, 1
	mov ch, byte [absTrack]
	mov cl, byte [absSector]
	mov dh, byte [absHead]
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
	add bx, 512
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
loading db "Loading system",24h
done db 0ah,0dh,"Done loading!",0ah,0dh,24h
reboot_msg db "Press any key to try again...",0ah,0dh,24h
error db "Disk read error.",0ah,0dh,24h
crlf db 0ah,0dh,24h
drive db 0
root dw 0x0200
absTrack dw 0
absSector dw 0
absHead dw 0
progress db 2eh,24h
fail db 0ah,0dh,"Disk read error :(",0ah,0dh,24h
not_supported db "You need atleast a 80386 processor.",0ah,0dh,24h

times 510-($-$$) db 0
dw 0xaa55
