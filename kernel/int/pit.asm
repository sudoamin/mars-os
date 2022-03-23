; The Programmable Interval Timer (PIT) chip
; https://wiki.osdev.org/Programmable_Interval_Timer

; PIT has four registers
; 1 mode command register (has four parts)
; 3 data registers

; I/O port     Usage
; 0x40         Channel 0 data port (read/write)
; 0x41         Channel 1 data port (read/write)
; 0x42         Channel 2 data port (read/write)
; 0x43         Mode/Command register (write only, a read is ignored)

; ONLY USE CHAN 0

pit_init:
      ; 0=0 1,2,3=010 4,5=11 6,7=00
      mov al, (1<<2) | (3<<4)
      ; use out instruction to write value in al to the register
      out 0x43, al

      ; channel 0 settings
      mov ax, 11931 ; 100 times per second. 1193182/100=11931
      ; because counter value is 16-bit
      out 0x40, al
      mov al, ah
      out 0x40, al

      ret