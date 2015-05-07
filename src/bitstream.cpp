#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "byteprint.h"
 
#define BUFFERSIZE 10 //4096

const unsigned int BITMASK[33] = {
  (1U<<0)-1U,
  (1U<<1)-1U,
  (1U<<2)-1U,
  (1U<<3)-1U,
  (1U<<4)-1U,
  (1U<<5)-1U,
  (1U<<6)-1U,
  (1U<<7)-1U,
  (1U<<8)-1U,
  (1U<<9)-1U,
  (1U<<10)-1U,
  (1U<<11)-1U,
  (1U<<12)-1U,
  (1U<<13)-1U,
  (1U<<14)-1U,
  (1U<<15)-1U,
  (1U<<16)-1U,
  (1U<<17)-1U,
  (1U<<18)-1U,
  (1U<<19)-1U,
  (1U<<20)-1U,
  (1U<<21)-1U,
  (1U<<22)-1U,
  (1U<<23)-1U,
  (1U<<24)-1U,
  (1U<<25)-1U,
  (1U<<26)-1U,
  (1U<<27)-1U,
  (1U<<28)-1U,
  (1U<<29)-1U,
  (1U<<30)-1U,
  (1U<<31)-1U,
  -1U
};


typedef struct bitstream {
  unsigned char *buffer;
  FILE* fd;
  size_t buffer_bitsize;
  size_t start_bit;
  size_t end_bit;
} BitStream;

/*
 * Initialize bitstream.
 *
 * Client is responsible for handling 
 */
BitStream bitstream_init(FILE *fp, size_t buffer_size) {
  BitStream bstr;

  unsigned char *buff = (unsigned char*)malloc(
      sizeof(unsigned char) * BUFFERSIZE);

  bstr.buffer = buff;
  bstr.fp = fp;
  bstr.buffer_bitsize = buffer_size * 8;
  bstr.start_bit = 0;
  bstr.end_bit = 0;
  return bstr;
}

/*
 * Cleaning up allocated memory.
 */
void bitstream_destroy(BitStream *bstr) {
  if (bstr->buffer) {
    free(bstr->buffer);
  }
  bstr->buffer_size = 0;
  bstr->end_bit = 0;
  bstr->start_bit = 0;
}

/* 
 * Reads "n" bits from the stream. Will increment the offset into the buffer by
 * "n", and returns how many bits were actually read.
 */
size_t bitstream_read(struct bitstream *bstr, size_t n, size_t *bits){
  size_t byte_offset = bstr->start_bit >> 3;
  if (bstr->start_bit + n > bstr->end_bit) {
    return false;
  }
  
  size_t bit_offset = bstr->start_bit  & 0xF;
  size_t value = *(size_t*)&bstr->buffer[byte_offset];
  value >>= bit_offset;
  *bits = value & BITMASK[n];
  bstr->start_bit += n;
  return true;
}

/*
 * Writes n bits from the bits value. Returns how many bits were written.
 */
size_t bitstream_write(struct bitstream *bstr, size_t n, size_t bits){

}

//  printf("%u\t%u\t"BINSHORT"\t"BINSHORT"\t"BINSHORT"\n", byte_offset, bit_offset, SHORTTOBINARY(value) , SHORTTOBINARY(BITMASK[n]), SHORTTOBINARY(*bits));
int main(void)
{
  BitStream b = bitstream_init(BUFFERSIZE);
  printf("%d %d\n\n", b.buffer_size, b.start_bit);
  int i;
  for (i = 0; i <= BUFFERSIZE; i++) {
    b.buffer[i] = 0xAA; //(char)i+128;
    printf(""BINBYTE" ", BYTETOBINARY(b.buffer[i]));
  }
  printf("\n");
  for (i = 0; i < 40; i++) {
    size_t bits;
    size_t read = bitstream_read(&b, 3U, &bits);
    printf("%d "BINBYTE"\n", read,  BYTETOBINARY(bits));
  }

/*
  for (i = 0; i < 16; i++) {
    printf(""BINSHORT"\n", SHORTTOBINARY(BITMASK[i]));
  }
*/
  return 0;
}
