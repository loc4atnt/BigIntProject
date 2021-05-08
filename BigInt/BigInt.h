#ifndef _BigInt_H_
#define _BigInt_H_

#include "Util.h"
#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct BigInt {
	byte* bytes;
	uint16_t byteCount;
	bool isHasSign;

	BigInt(byte *_bytes, uint16_t _byteCount, bool _isHasSign) : bytes(_bytes), byteCount(_byteCount), isHasSign(_isHasSign) {}
	BigInt() : bytes(NULL), byteCount(0), isHasSign(false) {}
	BigInt(const BigInt &a) : bytes(NULL), byteCount(0), isHasSign(false) {
		(*this) = a;
	}
	~BigInt() {
		free(bytes);
	}

	BigInt& operator=(const BigInt& a) {
		if (this != &a) {
			free(bytes);

			byteCount = a.byteCount;
			isHasSign = a.isHasSign;
			if (byteCount > 0) {
				bytes = (byte*)malloc(byteCount);
				memcpy_s(bytes, byteCount, a.bytes, byteCount);
			}
			else bytes = NULL;
		}
		return *this;
	}
};

BigInt binStrToBigInt(const char* binStr);
char* bigIntToBinStr(BigInt *b);
void addByteToBigInt(BigInt* bigInt, byte b);
void fillLastByteWithSignExcess(BigInt* i);
void addSignExcessBytes(BigInt* i, uint8_t amount);
void removeLastByteFromBigInt(BigInt* bigInt);
void reduceSignExcessBytes(BigInt* i);
void reduceSignExcessFromLastByte(BigInt* bigInt);

BigInt assignValue(int val);
BigInt oppositeNum(BigInt a);
BigInt operator + (BigInt a, BigInt b);
BigInt operator - (BigInt a, BigInt b);
BigInt operator ~ (BigInt a);
BigInt operator >> (BigInt a, int steps);
BigInt operator << (BigInt a, int steps);

bool readBit(BigInt *a, uint16_t idx);
void setBit(BigInt *a, uint16_t idx, bool bit);

#endif