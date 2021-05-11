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

#define PADDING_BASE '='
static char BASE32_ALPHABET[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";// 5BYTE 8 KY TU// TOI DA 7 PADDING =
static char BASE58_ALPHABET[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";// KHONG PADDING
static char BASE64_ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";// 3BYTE 4 KY TU// TOI DA 2 PADDING =

byte binStrToByte(const char* binStr);
char* byteToBinStr(byte b, bool isDispayFullBit = false);
void setBit(byte* b, uint8_t idx, bool bit);
bool readBit(byte b, uint8_t idx);
uint16_t max(uint16_t a, uint16_t b);
uint16_t min(uint16_t a, uint16_t b);
byte AndByte(byte b1, byte b2);
byte OrByte(byte b1, byte b2);
byte XorByte(byte b1, byte b2);
char valueToBase32Char(uint8_t val);
char valueToBase64Char(uint8_t val);
char valueToBase58Char(uint8_t val);
char* boolToString(bool b);
void insertCharFrontStr(char** str, int* strLen, char chr);

#endif