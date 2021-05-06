#include "ByteUtil.h"

#include <malloc.h>
#include <string.h>

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