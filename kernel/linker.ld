OUTPUT_FORMAT("elf64-x86-64")
ENTRY(init)

SECTIONS
{
      . = 0xffff800000009000;
      .text : {
            *(.text)
      }

      .rodata : {
            *(.rodata)
      }

      . = ALIGN(16);
      .data : {
            *(.data)
      }

      .bss : {
            *(.bss)
      }
}