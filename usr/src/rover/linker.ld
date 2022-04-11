OUTPUT_FORMAT("elf64-x86-64")
ENTRY(execute)

SECTIONS
{
      . = 0x400000;
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