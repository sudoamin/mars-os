# MarsOS
MarsOS is a simple 64-bit operating system developed for learning    
Feel free to contribute to MarsOS

# Build & Run   
Make sure you have installed [qemu-system-x86_64](https://archlinux.org/packages/extra/x86_64/qemu)
```console
make && make run && make clean
```

# TODO
1. Should I make a programming language and rewrite this OS ?
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
10. Write a loader to load larger kernels by the ATA
11. Append the ps1 to end of mars.img and load it at 0x20000
12. Complete console and Add a shell (named soil)


# Development notes
For now maximum size of MarsOS can be 100KB.    
This should be an unikernel?    
Its better cantact me befor starting development if you need help.    
