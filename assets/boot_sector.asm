section .text
    jmp entry
    nop
    ; 0-2     Jump to bootstrap (E.g. eb 3c 90; on i86: JMP 003E NOP.
    ;         One finds either eb xx 90, or e9 xx xx.
    ;         The position of the bootstrap varies.)
    ; 3-10    OEM name/version (E.g. "IBM  3.3", "IBM 20.0", "MSDOS5.0", "MSWIN4.0".
    ;         Various format utilities leave their own name, like "CH-FOR18".
    ;         Sometimes just garbage. Microsoft recommends "MSWIN4.1".)
    OEM_NAME: db "MAXIX BL"
    ;         /* BIOS Parameter Block starts here */
    ; 11-12   Number of bytes per sector (512)
    ;         Must be one of 512, 1024, 2048, 4096.
    bytes_per_sector: dw 0x200
    ; 13      Number of sectors per cluster (1)
    ;         Must be one of 1, 2, 4, 8, 16, 32, 64, 128.
    ;         A cluster should have at most 32768 bytes. In rare cases 65536 is OK.
    sec_per_cluster: db 16
    ; 14-15   Number of reserved sectors (1)
    ;         FAT12 and FAT16 use 1. FAT32 uses 32.
    reserved_sectors: dw 1
    ; 16      Number of FAT copies (2)
    number_fats: db 2
    ; 17-18   Number of root directory entries (224)
    ;         0 for FAT32. 512 is recommended for FAT16.
    root_dir_entries: dw 224
    ; 19-20   Total number of sectors in the filesystem (2880)
    ;         (in case the partition is not FAT32 and smaller than 32 MB)
    total_sectors: dw 2880
    ; 21      Media descriptor type (f0: 1.4 MB floppy, f8: hard disk; see below)
    media_descriptor: db 0xf0
    ; 22-23   Number of sectors per FAT (9)
    ;         0 for FAT32.
    sectors_per_fat: dw 9
    ; 24-25   Number of sectors per track (12)
    sectors_per_track: dw 12
    ; 26-27   Number of heads (2, for a double-sided diskette)
    heads: dw 2
    ; 28-29   Number of hidden sectors (0)
    hidden_sectors: dw 0

entry:
    jmp 07c0h:farjump_label
farjump_label:
    push cs
    pop ds
    push dx
    mov ax, WORD[reserved_sectors]
    add ax, WORD[hidden_sectors]
    inc ax
    mov cx, ax
    ;
    mov ax, WORD[sectors_per_fat]
    mul BYTE[number_fats]
    add ax, cx
    mov bx, ax
    ;
    mov ax,32
    mul [root_dir_entries]
    div dx, WORD[bytes_per_sector]
    ;
    mov ah, 02
    mov al, bl
    int 13h
    ;dx = data
    ;bx = root
    ;cx = fat


set_esbx:
mov bx,7f0h
mov es, bx
xor bx,bx
ret

read_seg: ;ax = segment to read
mov di,5 ; times we'll retry on disk I/O error
retry: div word [sectors_per_track]
mov cx,dx
inc cx
xor dx,dx
div word [heads]
mov dh, dl
mov ch, al
push cx
mov cl, 6
shr ah, cl
pop cx
or cl, ah
mov ah, 0201h
int 13h
jnc sectordone
dec di
jnz retry
mov al, cant_read_disk
display: lodsb ; AL=DS:[SI], increments SI by 1
mov ah,0eh
mov bx,7
int 10h
or al,al
jnz display
xor ax,ax
int 16h ; Keyboard interrupt / Wait for a key
int 19h ; re-enter the BIOS BSS bootstrap

sectordone:
ret



.data: 
    cant_read_disk: db "the Maxix bootloader wasn't able to read your Disk", 0h