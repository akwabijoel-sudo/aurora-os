#ifndef FAT32_H
#define FAT32_H

#include <stdint.h>

typedef struct {
    const char* name;
    uint32_t size;
    uint8_t is_dir;
    const char* content;
} VirtualFile;

typedef void (*file_browser_callback)(const char* name, uint32_t size, uint8_t is_dir);

void init_file_system();
void list_files(file_browser_callback callback);

#endif
