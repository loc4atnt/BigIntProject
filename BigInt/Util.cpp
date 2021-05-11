#include "Util.h"

#include <malloc.h>
#include <string.h>
#include <stdio.h>

byte binStrToByte(const char* binStr) {
	byte b = 0;
	uint8_t binStrLen = strlen(binStr);
	uint8_t bitCount = binStrLen > 8 ? 8 : binStrLen;
	//while (bitCount > 0 && binStr[binStrLen - bitCount] == '0') bitCount--;
	for (int i = 0; i < bitCount; i++) {
		setBit(&b, i, (binStr[binStrLen - 1 - i] != '0'));
	}
	return b;
}

char* byteToBinStr(byte b, bool isDispayFullBit) {
	char *binStr;
	uint8_t bitCount = 8;
	if (!isDispayFullBit) {
		while (bitCount > 1 && (!readBit(b, bitCount - 1))) bitCount--;
	}
	binStr = (char*)malloc(bitCount+1);
	for (int i = 0; i < bitCount; i++) {
		binStr[i] = readBit(b, bitCount - i - 1) ? '1' : '0';
	}
	binStr[bitCount] = '\0';
	return binStr;
}

void setBit(byte* b, uint8_t idx, bool bit) {
	byte maskByte = bit ? BYTE_MASK[idx] : (~BYTE_MASK[idx]);
	if (bit) (*b) = (*b) | maskByte;
	else (*b) = (*b) & maskByte;
}

bool readBit(byte b, uint8_t idx) {
	return (b & BYTE_MASK[idx]);
}

uint16_t max(uint16_t a, uint16_t b) {
	return a > b ? a : b;
}

uint16_t min(uint16_t a, uint16_t b) {
	return a < b ? a : b;
}

byte AndByte(byte b1, byte b2) {
	return (b1 & b2);
}

byte OrByte(byte b1, byte b2) {
	return (b1 | b2);
}
byte XorByte(byte b1, byte b2) {
	return (b1 ^ b2);
}

char valueToBase10Char(uint8_t val) {
	return BASE10_ALPHABET[val];
}
char valueToBase32Char(uint8_t val) {
	return BASE32_ALPHABET[val];
}

char valueToBase64Char(uint8_t val) {
	return BASE64_ALPHABET[val];
}

char valueToBase58Char(uint8_t val) {
	return BASE58_ALPHABET[val];
}

char* boolToString(bool b) {
	char* str = (char*)malloc(6);//true/false
	strcpy_s(str, 6, b ? "true" : "false");
	return str;
}

void insertCharFrontStr(char** str, int *strLen, char chr) {
	(*strLen)++;
	(*str) = (char*)realloc((*str), (*strLen)+1);
	memcpy_s((*str) +1, (*strLen), (*str), (*strLen));
	(*str)[0] = chr;
}
