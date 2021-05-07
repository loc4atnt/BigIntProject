#ifndef _BigInt_H_
#define _BigInt_H_

#include "Util.h"
#include <stdint.h>
#include <stdio.h>

struct BigInt {
	byte* bytes;
	uint16_t byteCount;
	bool isHasSign;

	BigInt& operator=(BigInt& a) {
		BigInt hmm = { NULL, 0, false };
		printf("vadk;fdaf :P");
		return hmm;
	}
};

static BigInt DefaultBigInt = {NULL, 0, false};

BigInt binStrToBigInt(const char* binStr);
char* bigIntToBinStr(BigInt b);
void addByteToBigInt(BigInt* bigInt, byte b);
void freeBigInt(BigInt* bigI);
void fillLastByteWithSignExcess(BigInt* i);
void addSignExcessBytes(BigInt* i, uint8_t amount);
void removeLastByteFromBigInt(BigInt* bigInt);
void reduceSignExcessBytes(BigInt* i);
void reduceSignExcessFromLastByte(BigInt* bigInt);

BigInt operator + (BigInt a, BigInt b);
void convertTwoComplement(BigInt& a);
BigInt oppositeNum(BigInt a);
BigInt operator - (BigInt a, BigInt b);
BigInt operator ~ (BigInt a);

#endif
