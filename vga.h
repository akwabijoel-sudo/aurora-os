#ifndef VGA_H
#define VGA_H
#include <stdint.h>

void vga_plot(int x, int y, uint8_t color);
void vga_clear(uint8_t color);
void vga_rect(int x, int y, int w, int h, uint8_t color);
void vga_putchar(int x, int y, char c, uint8_t color);
void draw_splash_screen();

#endif
