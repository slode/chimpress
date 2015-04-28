#include <map>
#include <string>

#include <stdio.h>
#include <limits.h>
 
int main(void)
{
  int dictSize = 256;

  std::map<std::string, short int> dictionary;
  for (short int i = 0; i < 256; i++)
    dictionary[std::string(1, i)] = i;

  char k;
  std::string w;
  while(fread(&k, 1, sizeof(char), stdin) == sizeof(char)) {
    char c = k;
    std::string wc = w + c;
    if (dictionary.count(wc)) {
      w = wc;
    } else {
      fwrite(&dictionary[w], 1, sizeof(short int), stdout); 

      if (dictSize < SHRT_MAX) {
        dictionary[wc] = dictSize++;
      }
      w = std::string(1, c);
    }
  }

  if (!w.empty())
    fwrite(&dictionary[w], 1, sizeof(short int), stdout); 

  return 0;
}
