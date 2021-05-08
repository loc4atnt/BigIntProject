#ifndef _Util_H_
#define _Util_H_

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
char* byteToBinStr(byte b, bool isDispayFullBit = false);
void setBit(byte* b, uint8_t idx, bool bit);
bool readBit(byte b, uint8_t idx);
uint16_t max(uint16_t a, uint16_t b);
uint16_t min(uint16_t a, uint16_t b);
byte AndByte(byte b1, byte b2);
byte OrByte(byte b1, byte b2);
byte XorByte(byte b1, byte b2);

#endif