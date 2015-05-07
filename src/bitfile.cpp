#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "byteprint.h"
 
#define BUFFERSIZE 10//4096*2*2

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
  FILE* fp;
  size_t buffer_bitsize;
  size_t start_bit;
  size_t end_bit;
} BitStream;

size_t _bitstream_read_helper(BitStream *bstr, size_t n, size_t *bits);
size_t _bitstream_buffer_read(BitStream *bstr);
size_t bitstream_read(BitStream *bstr, size_t n, size_t *bits);
size_t _bitstream_write_helper(BitStream *bstr, size_t n, size_t bits);
size_t _bitstream_buffer_write(BitStream *bstr);
size_t bitstream_write(struct bitstream *bstr, size_t n, size_t bits);

/*
 * Initialize bitstream.
 *
 * Client is responsible for handling file descriptor.
 */
BitStream bitstream_init(FILE *fp, size_t buffer_size) {
  BitStream bstr;

  unsigned char *buff = (unsigned char*)malloc(
      sizeof(unsigned char) * buffer_size);

  bstr.buffer = buff;
  bstr.fp = fp;
  bstr.buffer_bitsize = buffer_size << 3;
  bstr.start_bit = 0;
  bstr.end_bit = 0;
  return bstr;
}

/*
 * Cleaning up allocated memory.
 */
void bitstream_destroy(BitStream *bstr) {
  _bitstream_buffer_write(bstr);

  if (bstr->buffer) {
    free(bstr->buffer);
  }
  bstr->buffer_bitsize = 0;
  bstr->end_bit = 0;
  bstr->start_bit = 0;
}

size_t _bitstream_read_helper(BitStream *bstr, size_t n, size_t *bits) {
  if (bstr->start_bit + n > bstr->end_bit) {
    n = bstr->end_bit - bstr->start_bit;
  }
  size_t byte_offset = bstr->start_bit >> 3;
  size_t bit_offset = bstr->start_bit & 0x7;
  size_t value = *(size_t*)&bstr->buffer[byte_offset];
  value >>= bit_offset;
  *bits = value & BITMASK[n];
//  printf("%u\t%u\t"BINSHORT"\t"BINSHORT"\t"BINSHORT"\n", byte_offset, bit_offset, SHORTTOBINARY(value) , SHORTTOBINARY(BITMASK[n]), SHORTTOBINARY(*bits));
  bstr->start_bit += n;
  return n;
}

size_t _bitstream_buffer_read(BitStream *bstr) {
  size_t bytes_read = fread(
      bstr->buffer,
      sizeof(unsigned char),
      bstr->buffer_bitsize >> 3,
      bstr->fp);

  bstr->end_bit = bytes_read << 3;
  bstr->start_bit = 0;
//  printf("Filled buffer %u wanted %u, (%u, %u)\n", bytes_read, bstr->buffer_bitsize, bstr->start_bit, bstr->end_bit);
  return bytes_read;
}

/* 
 * Reads "n" bits from the stream. Will increment the offset into the buffer by
 * "n", and returns how many bits were actually read.
 */
size_t bitstream_read(BitStream *bstr, size_t n, size_t *bits){
  size_t bits_read = _bitstream_read_helper(bstr, n, bits);
  if (bits_read == n) {
    return bits_read;
  }
  // Refills the buffer, if 0 then the file is empty.
  if (_bitstream_buffer_read(bstr) == 0) {
    return bits_read;
  }
  // Adds the remaining bits from the new buffer
  size_t last_bits, bits_read_last;
  bits_read_last = _bitstream_read_helper(bstr, n-bits_read, &last_bits);
  *bits |= (last_bits << (bits_read));
  return bits_read_last + bits_read;
}

size_t _bitstream_write_helper(BitStream *bstr, size_t n, size_t bits) {
  if (bstr->end_bit + n > bstr->buffer_bitsize) {
    n = (bstr->buffer_bitsize) - bstr->end_bit;
  }
  size_t byte_offset = bstr->end_bit >> 3;
  size_t bit_offset = bstr->end_bit & 0x7;
  size_t *value = (size_t*)&bstr->buffer[byte_offset];
  *value |= ((bits & BITMASK[n]) << bit_offset);
  bstr->end_bit += n;
//  printf("%u\t%u\t"BINSHORT"\t"BINSHORT"\t"BINSHORT"\n",
//      byte_offset, bit_offset, SHORTTOBINARY(*value) , SHORTTOBINARY(BITMASK[n]), SHORTTOBINARY(bits));
  return n;
}

size_t _bitstream_buffer_write(BitStream *bstr) {
  if (bstr->start_bit == bstr->end_bit) {
    return 0;
  }
//  printf("Wrote buffer %u\n", bstr->end_bit>>3);
  size_t bytes_written = fwrite(
      bstr->buffer,
      sizeof(unsigned char),
      (bstr->end_bit >> 3),
      bstr->fp);

  bstr->end_bit = 0;
  bstr->start_bit = 0;
  return bytes_written;
}
/*
 * Writes n bits from the bits value. Returns how many bits were written.
 */
size_t bitstream_write(struct bitstream *bstr, size_t n, size_t bits){
  size_t bits_written = _bitstream_write_helper(bstr, n, bits);
  if (bits_written == n) {
    return bits_written;
  }

  // Refills the buffer, if 0 then the file is empty.
  _bitstream_buffer_write(bstr);

  // Adds the remaining bits from the new buffer
  size_t last_bits = bits >> bits_written;
  size_t bits_written_last = _bitstream_write_helper(bstr, n-bits_written, last_bits);
  return bits_written_last + bits_written;
}

int main(void)
{
  BitStream in = bitstream_init(stdin, BUFFERSIZE);
  BitStream out = bitstream_init(stdout, BUFFERSIZE);
  size_t bits, read;
  while ((read = bitstream_read(&in, 5U, &bits)) != 0) {
 //   printf("%d "BINBYTE" %c\n", read,  BYTETOBINARY(bits), bits);
    bitstream_write(&out, 5U, bits);
  }
  bitstream_destroy(&in);
  bitstream_destroy(&out);
  return 0;
}
