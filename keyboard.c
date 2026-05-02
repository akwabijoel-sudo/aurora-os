#include <stdint.h>
#include "vga.h"

// Basic Scan Code Set 1 to ASCII mapping
char scan_to_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' '
};

// Direct I/O port communication
uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void keyboard_handler_polling() {
    // Check Status Register (Port 0x64), bit 0 (Output Buffer Full)
    if (inb(0x64) & 0x1) {
        uint8_t scancode = inb(0x60); // Read scan code from Data Port

        // Only handle "Key Pressed" events (scancodes below 0x80)
        if (scancode < 0x80) {
            char c = scan_to_ascii[scancode];
            if (c > 0) {
                // For now, let's just draw a pixel at a spot unique to that key
                put_pixel(100 + scancode, 100, scancode); 
            }
        }
    }
}
