#ifndef _BigInt_H_
#define _BigInt_H_

#include "Util.h"
#include <stdint.h>

struct BigInt {
	byte* bytes;
	uint16_t byteCount;
	bool isHasSign;
};

static BigInt DefaultBigInt = {NULL, 0, false};

BigInt binStrToBigInt(const char* binStr);
char* bigIntToBinStr(BigInt b);
void addByteToBigInt(BigInt* bigInt, byte b);
void freeBigInt(BigInt bigI);

BigInt operator + (BigInt a, BigInt b);
void convertTwoComplement(BigInt& a);
BigInt oppositeNum(BigInt a);
BigInt operator - (BigInt a, BigInt b);

#endif
