[bits 16]
gdt_start:
    dq 0x0          ; Null descriptor
gdt_code: 
    dw 0xffff, 0x0, 10011010b, 11001111b, 0x0
gdt_data: 
    dw 0xffff, 0x0, 10010010b, 11001111b, 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

[bits 32]
switch_to_pm:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load GDT
    mov eax, cr0
    or eax, 0x1             ; Set Protected Mode bit
    mov cr0, eax
    jmp CODE_SEG:init_pm    ; Far jump to 32-bit

init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ret                     ; Return to bootloader (now in 32-bit!)
