#include <stdio.h>


int main(void)
{
  int c, prev;
  int char_count;

  prev = EOF; 
  while((c = getchar()) != EOF) {
    if (c != prev) {
      putchar(c);
    } else {
      scanf("%d_", &char_count);
      while (char_count-- > 0) {
        putchar(c);
      }
    }
    prev = c;
  }

  return 0;
}
