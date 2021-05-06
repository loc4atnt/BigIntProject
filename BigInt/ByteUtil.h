#ifndef _ByteUtil_H_
#define _ByteUtil_H_

#include <stdint.h>

typedef unsigned char byte;

static byte BYTE_MASK[8] = { 0b00000001,
							 0b00000010,
							 0b00000100,
							 0b00001000,
							 0b00010000,
							 0b00100000,
							 0b01000000,
							 0b10000000 };

byte binStrToByte(const char* binStr);
char* byteToBinStr(byte b);
void setBit(byte* b, uint8_t idx, bool bit);
bool readBit(byte b, uint8_t idx);

#endif