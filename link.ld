ENTRY(kernel_main)
OUTPUT_FORMAT(binary)

SECTIONS
{
    . = 0x7e00;
    .text : {
        *(.text.kernel_main)
        *(.text)
    }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
    
    /* This fills the file with zeros until it's a multiple of 512 */
    . = ALIGN(512);
}
