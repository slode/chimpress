#include <stdio.h>


int main(void)
{
  int c, prev;
  int char_count;

  char_count = 0;
  prev = EOF; 
  while((c = getchar()) != EOF) {
    if (c != prev) {
      if (char_count > 0) {
        printf("%d_", char_count);
        char_count = 0;
      }
      putchar(c);
    } else {
      if (char_count == 0) {
        putchar(c);
      }
      char_count++;
    }
    prev = c;
  }

  return 0;
}
