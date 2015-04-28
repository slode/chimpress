#include <map>
#include <string>

#include <stdio.h>
#include <limits.h>

int main(void)
{
  short int k;
  int dictSize = 256;

  std::map<short int, std::string> dictionary;
  for (short int i = 0; i < 256; i++) {
    dictionary[i] = std::string(1, i);
  }

  std::string entry, w;

  if (fread(&k, 1, sizeof(short int), stdin) == sizeof(short int)) {
    w = std::string(1, char(k));  
    fwrite(&w[0], w.size(), sizeof(char), stdout); 
  }
  else {
    return -1;
  }

  while(fread(&k, 1, sizeof(short int), stdin) == sizeof(short int)) {

    if (dictionary.count(k)) {
      entry = dictionary[k];
    } 
    else if (k == dictSize) {
      entry = w + w[0];
    } 
    else {
      return -2;
    }

    fwrite(&entry[0], entry.size(), sizeof(char), stdout); 

    if (dictSize < SHRT_MAX) {
      dictionary[dictSize++] = w + entry[0];
    }
    w = entry;
  }

  return 0;
}
