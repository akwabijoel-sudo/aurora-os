#include "vga.h"
#include <stdint.h>

void init_mouse();
uint8_t inb(uint16_t port);

// Keyboard Map with Enter (0x1C) and Backspace (0x0E)
char kbd_map[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' '
};

// --- This shows the "Found" window ---
void show_search_results() {
    vga_rect(45, 80, 230, 80, 15); // White Result Box
    vga_putchar(60, 90, 'W', 4);  // WEB (Red)
    vga_putchar(70, 90, 'E', 4);
    vga_putchar(80, 90, 'B', 4);
    vga_putchar(100, 90, 'F', 1); // FOUND (Blue)
    vga_putchar(110, 90, 'O', 1);
    vga_putchar(120, 90, 'U', 1);
    vga_putchar(130, 90, 'N', 1);
    vga_putchar(140, 90, 'D', 1);
}

__attribute__((section(".text.kernel_main")))
void kernel_main() {
    draw_splash_screen();
    uint8_t desktop_color = 2; 
    vga_clear(desktop_color);
    vga_rect(0, 180, 320, 20, 8); 
    vga_rect(2, 182, 30, 16, 7);  
    vga_rect(55, 182, 16, 16, 1); 

    init_mouse();
    int mouse_x = 160, mouse_y = 100, search_open = 0, typing_pos = 60;

    while(1) {
        uint8_t status = inb(0x64);
        if (status & 1) {
            if (status & 0x20) { // Mouse Logic
                uint8_t m_stat = inb(0x60); 
                int8_t dx = (int8_t)inb(0x60);
                int8_t dy = (int8_t)inb(0x60);
                
                uint8_t erase_color = (mouse_y >= 180) ? 8 : desktop_color;
                if (search_open && mouse_x >= 40 && mouse_x <= 280 && mouse_y >= 30 && mouse_y <= 170) {
                    erase_color = 1; 
                    if (mouse_x >= 50 && mouse_x <= 270 && mouse_y >= 50 && mouse_y <= 70) erase_color = 15;
                }
                vga_rect(mouse_x, mouse_y, 4, 4, erase_color);
                
                mouse_x += dx; mouse_y -= dy;
                if (mouse_x < 0) mouse_x = 0; if (mouse_x > 316) mouse_x = 316;
                if (mouse_y < 0) mouse_y = 0; if (mouse_y > 196) mouse_y = 196;

                if ((m_stat & 1) && mouse_x >= 55 && mouse_x <= 71 && mouse_y >= 182) {
                    search_open = !search_open;
                    vga_clear(desktop_color);
                    vga_rect(0, 180, 320, 20, 8);
                    vga_rect(2, 182, 30, 16, 7);
                    vga_rect(55, 182, 16, 16, 1);
                    if (search_open) {
                        vga_rect(40, 30, 240, 140, 1);
                        vga_rect(50, 50, 220, 20, 15);
                        typing_pos = 60;
                    }
                    for(volatile int i = 0; i < 2000000; i++);
                }
            } else { // Keyboard Logic
                uint8_t sc = inb(0x60);
                // IF ENTER IS PRESSED (Scancode 0x1C)
                if (sc == 0x1C) {
                    show_search_results();
                } else if (search_open && sc < sizeof(kbd_map)) {
                    char key = kbd_map[sc];
                    if (key != 0 && key != '\b' && key != '\n') {
                        vga_putchar(typing_pos, 56, key, 0);
                        typing_pos += 10;
                    }
                }
            }
        }
        vga_rect(mouse_x, mouse_y, 4, 4, 15);
    }
}
