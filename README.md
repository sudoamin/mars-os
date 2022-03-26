# MARS OS    

# Build & Run   
Make sure you have installed [qemu-system-x86_64](https://archlinux.org/packages/extra/x86_64/qemu)
```console
make && make run && make clean
```

# TODO
1. Move kernel to 0x100000
2. Save registers in interrupt handlers. the pusha, popa instructions not supported in 64-bit mode
3. Rewrite the IDT in the C   
