; Get memory map with BIOS function
; Query System Address Map
; https://uefi.org/specs/ACPI/6.4/15_System_Address_Map_Interfaces/int-15h-e820h---query-system-address-map.html

; interrupt 15 returns a list of memory blocks
; the structure of blocks 
; 0 base address, the physical address of the start of the range being specified
; 8 length, the length is the physical contiguous length in bytes of a range being specified
; 16 type, describes the usage of the described address range as defined in Address Range Types

get_mem_info:
      mov eax, 0xe820
      mov edx, 0x534d4150
      mov ecx, 20
      mov dword[0x8000], 0
      mov edi, 0x8008
      xor ebx, ebx
      int 0x15
      jc mem_info_failed ; service e820 is not available

loop_mem_info:
      add edi, 20 ; 0x8008 + 20 to get next memory block (each block is 20 bytes)
      inc dword[0x8000]
      test ebx, ebx
      jz mem_info_done

      mov eax, 0xe820
      mov edx, 0x534d4150
      mov ecx, 20
      int 0x15
      jnc loop_mem_info

mem_info_failed:
      mov si, MSG_MEM_INFO_FAILED
      call print

      jmp end

mem_info_done:
      ret

MSG_MEM_INFO_FAILED: db "* Failed to get memory info", 0ah, 0dh, 0