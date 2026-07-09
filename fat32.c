#include "fat32.h"

// 4 distinct file descriptors to prevent the shell from breaking array indices
static VirtualFile file0;
static VirtualFile file1;
static VirtualFile file2;
static VirtualFile file3;

void init_file_system() {
    file0.name = "Documents";
    file0.size = 0;
    file0.is_dir = 1;
    file0.content = "";

    file1.name = "readme.txt";
    file1.size = 22;
    file1.is_dir = 0;
    file1.content = "Welcome to Aurora OS!";

    file2.name = "version.txt";
    file2.size = 11;
    file2.is_dir = 0;
    file2.content = "Version 1.0";

    file3.name = "system.sys";
    file3.size = 1024;
    file3.is_dir = 0;
    file3.content = "[Kernel Configuration Data]";
}

void list_files(file_browser_callback callback) {
    // Sequentially forward each independent object descriptor to your GUI logic
    callback(file0.name, file0.size, file0.is_dir);
    callback(file1.name, file1.size, file1.is_dir);
    callback(file2.name, file2.size, file2.is_dir);
    callback(file3.name, file3.size, file3.is_dir);
}
