#include <stdio.h>
#include <limits.h>
 
#define BUFFERSIZE 4096

struct bitstream {
  unsigned char buffer[BUFFERSIZE];
  size_t start_bit;
};

#define BINBYTE "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) 

#define BINSHORT "%d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d"
#define SHORTTOBINARY(byte)  \
  (byte & 0x8000 ? 1 : 0), \
  (byte & 0x4000 ? 1 : 0), \
  (byte & 0x2000 ? 1 : 0), \
  (byte & 0x1000 ? 1 : 0), \
  (byte & 0x800 ? 1 : 0), \
  (byte & 0x400 ? 1 : 0), \
  (byte & 0x200 ? 1 : 0), \
  (byte & 0x100 ? 1 : 0), \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) 

const unsigned int BITMASK[32] = {
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
  (1U<<32)-1U
};
/* 
 * Reads "n" bits from the stream. Will increment the offset into the buffer by
 * "n", and returns how many bits were actually read.
 */
size_t get_bits(struct bitstream *bstr, size_t n, size_t *bits){
  size_t byte_offset = bstr->start_bit >> 3;
  size_t bit_offset = bstr->start_bit  & 0xF;
  size_t value = *(size_t*)&bstr->buffer[byte_offset];
  value >>= bit_offset;
  *bits = value & BITMASK[n];
  bstr->start_bit += n;
  printf("%u\t%u\t"BINSHORT"\t"BINSHORT"\t"BINSHORT"\n", byte_offset, bit_offset, SHORTTOBINARY(value) , SHORTTOBINARY(BITMASK[n]), SHORTTOBINARY(*bits));
  return n;
}
/*
 * Writes n bits from the bits value. Returns how many bits were written.
 */
size_t put_bits(struct bitstream *bstr, size_t n, size_t bits){

}

int main(void)
{
  struct bitstream b;
  b.start_bit = 0;
  int i;
  for (i = 0x0; i < 20; i++) {
    b.buffer[i] = (char)i+128;
    printf(""BINBYTE" ", BYTETOBINARY(b.buffer[i]));
  }
  printf("\n");
  for (i = 0; i < 40; i++) {
    size_t bits;
    get_bits(&b, 20U, &bits);
  }

  for (i = 0; i < 16; i++) {
    printf(""BINSHORT"\n", SHORTTOBINARY(BITMASK[i]));
  }
  return 0;
}
