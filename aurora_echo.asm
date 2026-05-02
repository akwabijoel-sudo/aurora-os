[bits 16]
[section .text]
global _start

_start:
    mov ax, 0x0013
    int 0x10

    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp 0x08:init_pm

[bits 32]
init_pm:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Jump to the Kernel at 0x8000
    extern kernel_main
    call 0x8000
    jmp $

gdt_start: dq 0x0
gdt_code:  dw 0xffff, 0x0, 10011010b, 11001111b, 0x0
gdt_data:  dw 0xffff, 0x0, 10010010b, 11001111b, 0x0
gdt_end:
gdt_descriptor: dw gdt_end - gdt_start - 1
                dd gdt_start

times 510-($-$$) db 0
dw 0xaa55
