#ifndef PS2_H
#define PS2_H

#include <stdint.h>

// NOTE:
// The scan code is sent to the handler when we press a key
// and realse a key, so we have two scenarios.
// Key down and key up or make code and break code.
// the break code is the code of corresponding make code with 80
// we have one byte code for a key, multiple-byte code used for function keys
// which will be sent one byte at time
// most of the multiple-byte key code comes with E0 first

// KeyboardBuffer is a circular queue.
struct kbrd_buf {
  char buffer[500];
  int front;
  int end;
  int size;
};

#define E0_SIGN (1 << 0)
#define SHIFT (1 << 1)
#define CAPS_LOCK (1 << 2)

char read_key_buf(void);
void ps2_handler(void);
unsigned char in_byte(uint16_t port);

#endif