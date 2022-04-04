# MarsOS
MarsOS is a simple 64-bit operating system developed for learning    
Feel free to contribute to MarsOS

# Build & Run   
Make sure you have installed [qemu-system-x86_64](https://archlinux.org/packages/extra/x86_64/qemu)
```console
make && make run && make clean
```

# TODO
1. Move kernel to 0x9000 ✓
2. Removed
3. Update Makefile   
4. Write Documents
5. Memory managment
    - Heap(malloc, free)
6. Process managment
    - Scheduler
    - Wait/Exit
    - Sleep
    - Fork/Exec
    - Syscalls
7. Filesystem
    - ATA
    - FAT16
    - FD
8. Network stack
9. Drivers
    - RTL8139
10. Write a loader to loading larger kernels and using the ATA
11. Append the ps1 to end of mars.img and load it at 0x20000
12. Complete console and Add a shell (named soil)
