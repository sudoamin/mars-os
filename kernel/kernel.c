
int kmain(void) {
      char* p = (char*)0xb8000;
      p[50] = 'c';
      p[51] = 0xa;

      return 0;
}