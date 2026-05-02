[bits 16]
[org 0x7c00]

start:
    ; 1. Set VGA Mode 13h
    mov ax, 0x0013
    int 0x10

    ; 2. LOAD C KERNEL FROM DISK (This pulls the Green code into RAM)
    mov ah, 0x02
    mov al, 30          ; Load 30 sectors
    mov ch, 0
    mov dh, 0
    mov cl, 2
    mov bx, 0x7e00      ; Destination address
    int 0x13

    ; 3. Switch to Protected Mode
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Far Jump to 32-bit
    jmp 0x08:init_pm

[bits 32]
init_pm:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000    ; Setup stack for C

    ; 4. JUMP TO C KERNEL (Replaces the 'Paint Red' part)
    jmp 0x7e00

; --- STABLE ALIGNED GDT (Your working version) ---
align 8
gdt_start:
    dq 0x0                  
gdt_code:
    dw 0xFFFF, 0x0000, 0x9A00, 0x00CF
gdt_data:
    dw 0xFFFF, 0x0000, 0x9200, 0x00CF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55
