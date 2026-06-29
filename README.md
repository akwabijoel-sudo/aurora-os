# 🌅 Aurora OS

A custom x86 32-bit Protected Mode operating system built completely from scratch, featuring an independent bootloader, monolithic C kernel, and native hardware drivers.

## 🚀 System Architecture Features
* **Custom Bootloader (`boot.asm`):** Handles the transition from 16-bit Real Mode to 32-bit Protected Mode.
* **Kernel Core (`kernel.c`):** Monolithic systems core managing execution and kernel loops.
* **Graphics Subsystem (`vga.c`):** Low-level interface writing directly to VGA Mode 13h (320x200 linear frame buffer).
* **Hardware Input Drivers (`mouse.c`):** Custom Interrupt Service Routines (ISRs) handling low-level PS/2 Keyboard and Mouse inputs.

---

## 🛠️ Compilation & Build Pipeline

To compile the kernel objects, link them with the custom linker map, attach the bootloader sector, and run the operating system image in QEMU, run the following command chain:

```bash
# Compile C Modules into Freestanding 32-bit Objects
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -c vga.c -o vga.o && \
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -c mouse.c -o mouse.o && \
gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -c kernel.c -o kernel.o && \

# Link Kernel Objects using Custom Linker Script into a Flat Binary
ld -m elf_i386 -T link.ld -o kernel.bin vga.o mouse.o kernel.o --oformat binary -no-pie && \

# Assemble the 16-bit Bootloader Stage 1
nasm -f bin boot.asm -o boot.bin && \

# Create the Final Bootable Disk Image
cat boot.bin kernel.bin > aurora.img && \

# Pad the OS Image File Size (10KB)
truncate -s 10240 aurora.img && \

# Launch Aurora OS in the QEMU Emulator
qemu-system-i386 -drive format=raw,file=aurora.img
```

---

## 💾 Repository Structure
* `boot.asm` - 16-bit real mode initialization, Protected Mode switch, and A20 line toggle.
* `link.ld` - Linker script defining the kernel memory layout and text sections.
* `kernel.c` - Main entry point and event handling system.
* `vga.c` - Video graphics array pixel mapping and cursor rendering functions.
* `mouse.c` - PS/2 mouse port I/O communication and driver logic.
