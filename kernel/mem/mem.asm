global load_cr3

; load the address of PML4 table to cr3 register
load_cr3:
      mov rax, rdi
      mov cr3, rax
      ret