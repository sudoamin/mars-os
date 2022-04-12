#include <drivers/include/ps2.h>
#include <kernel/include/console.h>
#include <kernel/include/io.h>
#include <kernel/include/proc.h>

static unsigned char shift_code[256] = {
    [0x2A] = SHIFT, [0x36] = SHIFT, [0xAA] = SHIFT, [0xB6] = SHIFT};

static unsigned char lock_code[256] = {[0x3A] = CAPS_LOCK};

static char key_map[256] = {
    0,   0,   '1',  '2', '3',  '4', '5', '6',  '7', '8', '9', '0',
    '-', '=', '\b', 0,   'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
    'o', 'p', '[',  ']', '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
    'j', 'k', 'l',  ';', '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm',  ',', '.',  '/', 0,   '*',  0,   ' '};

static char shift_key_map[256] = {
    0,   1,   '!',  '@',  '#',  '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', '\t', 'Q',  'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{',  '}',  '\n', 0,   'A', 'S', 'D', 'F', 'G', 'H',
    'J', 'K', 'L',  ':',  '"',  '~', 0,   '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M',  '<',  '>',  '?', 0,   '*', 0,   ' '};

static struct kbrd_buf key_buf = {{0}, 0, 0, 500};
static unsigned int flag;

static void write_key_buf(char ch) {
  int front = key_buf.front;
  int end = key_buf.end;
  int size = key_buf.size;

  // check if the buffer is full.
  // if the end is one element away reaching to the front,
  // it means that the buffer is full and we simply return
  if ((end + 1) % size == front) {
    return;
  }
  // write the data in the buffer
  // update the end and write it back to the key buffer
  key_buf.buffer[end++] = ch;
  // if the end is pointing to the last element,
  // we will start from the beginning again
  key_buf.end = end % size;
}

char read_key_buf(void) {
  int front = key_buf.front;

  // if the buffer is empty, put the process into sleep
  if (front == key_buf.end) {
    proc_sleep(10);
  }

  // return the keys and update front to point to the next location
  key_buf.front = (key_buf.front + 1) % key_buf.size;
  return key_buf.buffer[front];
}

static char kbrd_read_handler(void) {
  unsigned char scan_code;
  char ch;

  scan_code = inb(0x60);

  // if the scan code is starts with E0,
  // we will add E0 sign to the flag and return 0
  // meaning that the key is not valid
  if (scan_code == 0xE0) {
    flag |= E0_SIGN;
    return 0;
  }

  // if the key is not equal to E0,
  // but the E0 is set in the flag,
  // it means that the last scan code is E0.
  // so this key is a function key which is not used.
  // clear the E0 flag for testing the following keys.
  if (flag & E0_SIGN) {
    flag &= ~E0_SIGN;
    return 0;
  }

  // handling key up
  if (scan_code & 0x80) {
    flag &= ~(shift_code[scan_code]);
    return 0;
  }

  flag |= shift_code[scan_code];
  flag ^= lock_code[scan_code];

  if (flag & SHIFT) {
    ch = shift_key_map[scan_code];
  } else {
    ch = key_map[scan_code];
  }

  if (flag & CAPS_LOCK) {
    if ('a' <= ch && ch <= 'z')
      ch -= 32;
    else if ('A' <= ch && ch <= 'Z')
      ch += 32;
  }

  return ch;
}

void ps2_handler(void) {
  char ch = kbrd_read_handler();

  if (ch > 0) {
    // write the key to the buffer
    write_key_buf(ch);
    // wake up the proccess waiting for the keyboard.
    // when a program wants to read a key and there is no key in the buffer,
    // we will put it into sleep.
    // 10 represents the processes waiting for the keyboard IO
    proc_wake_up(10);
  }
}