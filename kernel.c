#include "vga.h"
#include "fat32.h"
#include <stdint.h>

void init_mouse();

static inline uint8_t kernel_inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

char kbd_map[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' '
};

static int file_y_cursor = 40;

void gui_print_string(int x, int y, const char* str, uint8_t color) {
    int cur_x = x;
    for (int i = 0; str[i] != '\0'; i++) {
        vga_putchar(cur_x, y, str[i], color);
        cur_x += 8;
    }
}

void draw_file_item(const char* name, uint32_t size, uint8_t is_dir) {
    if (is_dir) {
        vga_rect(45, file_y_cursor, 10, 10, 14);
        gui_print_string(60, file_y_cursor, name, 15);
    } else {
        vga_rect(45, file_y_cursor, 10, 10, 7);
        gui_print_string(60, file_y_cursor, name, 15);
    }
    file_y_cursor += 15;
}

void show_search_results() {
    vga_rect(45, 80, 230, 80, 15);
    vga_putchar(60, 90, 'W', 4);
    vga_putchar(70, 90, 'E', 4);
    vga_putchar(80, 90, 'B', 4);
    vga_putchar(100, 90, 'F', 1);
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
    vga_rect(55, 182, 16, 16, 1); // Original blue button stays untouched
    vga_rect(80, 182, 16, 16, 7); // Beige button for our file explorer

    init_mouse();
    init_file_system();

    int mouse_x = 160, mouse_y = 100, search_open = 0, typing_pos = 60;

    while(1) {
        uint8_t status = kernel_inb(0x64);
        if (status & 1) {
            if (status & 0x20) { // Your EXACT original mouse decoding streams
                uint8_t m_stat = kernel_inb(0x60);
                int8_t dx = (int8_t)kernel_inb(0x60);
                int8_t dy = (int8_t)kernel_inb(0x60);

                // Your EXACT original smart eraser tracking logic
                uint8_t erase_color = (mouse_y >= 180) ? 8 : desktop_color;
                if (search_open && mouse_x >= 40 && mouse_x <= 280 && mouse_y >= 30 && mouse_y <= 170) {
                    erase_color = 1;
                    if (mouse_x >= 50 && mouse_x <= 270 && mouse_y >= 50 && mouse_y <= 70) erase_color = 15;
                }
                
                // Erase the old cursor position using the background color it was standing on
                vga_rect(mouse_x, mouse_y, 4, 4, erase_color);

                mouse_x += dx; mouse_y -= dy;
                if (mouse_x < 0) mouse_x = 0; if (mouse_x > 316) mouse_x = 316;
                if (mouse_y < 0) mouse_y = 0; if (mouse_y > 196) mouse_y = 196;

                // --- HANDLE WINDOW OBJECT CLICKS ---
                if ((m_stat & 1) && search_open) {
                    // Red window close cross (X) handler
                    if (mouse_x >= 266 && mouse_x <= 276 && mouse_y >= 34 && mouse_y <= 44) {
                        search_open = 0;
                        vga_clear(desktop_color);
                        vga_rect(0, 180, 320, 20, 8);
                        vga_rect(2, 182, 30, 16, 7);
                        vga_rect(55, 182, 16, 16, 1);
                        vga_rect(80, 182, 16, 16, 7);
                        for(volatile int i = 0; i < 2000000; i++);
                        continue;
                    }

                    // Click tracking inside our file lists text columns
                    if (mouse_x >= 45 && mouse_x <= 250) {
                        if (mouse_y >= 80 && mouse_y < 140) {
                            int clicked_file = (mouse_y - 80) / 15;

                            if (clicked_file == 1) { 
                                vga_rect(60, 100, 200, 50, 15); 
                                gui_print_string(65, 110, "WELCOME TO AURORA OS!", 0); 
                            }
                            if (clicked_file == 2) { 
                                vga_rect(60, 100, 200, 50, 15);
                                gui_print_string(65, 110, "VERSION 1.0", 0);
                            }
                            for(volatile int i = 0; i < 2000000; i++);
                        }
                    }
                }

                // --- HANDLE BEIGE ICON TASKBAR TOGGLE ---
                if ((m_stat & 1) && mouse_x >= 80 && mouse_x <= 96 && mouse_y >= 182) {
                    search_open = !search_open;
                    vga_clear(desktop_color);
                    vga_rect(0, 180, 320, 20, 8);
                    vga_rect(2, 182, 30, 16, 7);
                    vga_rect(55, 182, 16, 16, 1);
                    vga_rect(80, 182, 16, 16, 7);
                    if (search_open) {
                        vga_rect(40, 30, 240, 140, 1);
                        vga_rect(50, 50, 220, 20, 15);
                        
                        // Close button rectangle box layout
                        vga_rect(266, 34, 10, 10, 4);
                        vga_putchar(268, 35, 'X', 15);
                        
                        file_y_cursor = 80;
                        list_files(draw_file_item);
                        typing_pos = 60;
                    }
                    for(volatile int i = 0; i < 2000000; i++);
                }
            } else { // Keyboard Logic
                uint8_t sc = kernel_inb(0x60);
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
        // Draw your actual mouse cursor tracking box (Color 15 = White)
        vga_rect(mouse_x, mouse_y, 4, 4, 15);
    }
}
