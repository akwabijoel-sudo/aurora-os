[org 0x7C00]
bits 16

start:
    ; Print welcome message
    mov si, message
    call print_string

main_loop:
    ; Wait for key press
    mov ah, 0x00
    int 0x16        ; BIOS keyboard interrupt

    ; Echo the key to the screen
    mov ah, 0x0E
    int 0x10        ; BIOS teletype output

    jmp main_loop   ; keep looping

; -------------------------
; Print string routine
print_string:
    mov ah, 0x0E
.next:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .next
.done:
    ret

message db 'Aurora OS v0.2 - Type something: ', 0

times 510-($-$$) db 0
dw 0xAA55
