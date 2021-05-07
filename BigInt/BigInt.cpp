#include "BigInt.h"
#include <malloc.h>
#include <string.h>
#include <math.h> 

BigInt binStrToBigInt(const char* binStr) {
	BigInt bigI = DefaultBigInt;
	uint16_t bitLen = strlen(binStr);

	byte tmpByte;
	char* tmpBinStr = (char*)malloc(9);
	int i = bitLen;
	int convertedBitAmount;
	do {
		i -= 8;
		if (i < 0) {
			convertedBitAmount = 8 + i;
			i = 0;
		}
		else
			convertedBitAmount = 8;

		strncpy_s(tmpBinStr, 9, binStr + i, convertedBitAmount);
		tmpBinStr[convertedBitAmount] = '\0';
		tmpByte = binStrToByte(tmpBinStr);
		addByteToBigInt(&bigI, tmpByte);
	} while (i>0);

	free(tmpBinStr);
	return bigI;
}

char* bigIntToBinStr(BigInt b) {
	if (b.byteCount == 0) return NULL;

	char* tmpBinStr = byteToBinStr(b.bytes[b.byteCount - 1]);
	int binStrBuffLen = strlen(tmpBinStr) + 1;
	char* binStr = (char*)malloc(binStrBuffLen);
	strcpy_s(binStr, binStrBuffLen, tmpBinStr);

	for (int i = b.byteCount - 2; i >= 0; i--) {
		free(tmpBinStr);
		tmpBinStr = byteToBinStr(b.bytes[i], true);
		binStrBuffLen += 8;
		binStr = (char*)realloc(binStr, binStrBuffLen);
		strcat_s(binStr, binStrBuffLen, tmpBinStr);
	}
	free(tmpBinStr);

	return binStr;
}

void addByteToBigInt(BigInt* bigInt, byte b) {
	bigInt->byteCount++;
	bigInt->bytes = (byte*)realloc(bigInt->bytes, bigInt->byteCount);
	bigInt->bytes[bigInt->byteCount - 1] = b;
}

void freeBigInt(BigInt* bigI) {
	free(bigI->bytes);
	*bigI = DefaultBigInt;
}

BigInt operator+(BigInt a, BigInt b)
{
	BigInt res;
	short maxByte = max(a.byteCount, b.byteCount);
	if (a.isHasSign == b.isHasSign) {
		res.byteCount = maxByte + 1;
	}
	else res.byteCount = maxByte;
	res.bytes = (byte*)malloc(res.byteCount);	
	if (a.byteCount != b.byteCount) {
		BigInt* p = (a.byteCount > b.byteCount) ? &b : &a;
		addSignExcessBytes(p, (byte)fabs(a.byteCount - b.byteCount));
	}	
	res.bytes[0] = a.bytes[0] + b.bytes[0];
	bool bitOverflow = (a.bytes[0] + b.bytes[0] > 255) ? 1 : 0;
	
	for (int i = 1; i < maxByte; i++) {
		res.bytes[i] = a.bytes[i] + b.bytes[i] + (bitOverflow ? 1 : 0);
		if ((a.bytes[i] + b.bytes[i] + (bitOverflow ? 1 : 0)) > 255)  bitOverflow = 1;
		else bitOverflow = 0;
	}
	if (a.isHasSign == b.isHasSign) {
		BigInt* p = &res;
		if (!bitOverflow) {
			removeLastByteFromBigInt(p);
		}
		else {
			res.bytes[res.byteCount - 1] = 1;
			fillLastByteWithSignExcess(p);
		}
	}	
	return res;
}
void convertTwoComplement(BigInt& res)
{
	for (int i = 0; i < res.byteCount; i++) {
		res.bytes[i] = ~res.bytes[i];
	}
	byte v = 1;
	byte* p = &v;
	BigInt i = { p,1,0 };
	res = res + i;
}

BigInt oppositeNum(BigInt a)
{
	BigInt res = a;
	res.isHasSign = 1 - res.isHasSign;
	convertTwoComplement(res);
	return res;
}

BigInt operator-(BigInt a, BigInt b) {
	BigInt res;
	res = a + oppositeNum(b);
	return res;

}

void fillLastByteWithSignExcess(BigInt* i) {
	if (!i->isHasSign) return;
	uint8_t lastHighBitIdxInLastByte = 7;
	while (lastHighBitIdxInLastByte >= 0 && (readBit(i->bytes[i->byteCount - 1], lastHighBitIdxInLastByte) == 0))
		lastHighBitIdxInLastByte--;
	for (int idx = lastHighBitIdxInLastByte + 1; idx < 8; idx++) {
		setBit(&i->bytes[i->byteCount - 1], idx, 1);
	}
}

void addSignExcessBytes(BigInt* i, uint8_t amount) {
	if (i->isHasSign) fillLastByteWithSignExcess(i);
	byte signExcess = i->isHasSign ? 0b11111111 : 0b00000000;
	for (int idx = 0; idx < amount; idx++) {
		addByteToBigInt(i, signExcess);
	}
}

void removeLastByteFromBigInt(BigInt* bigInt) {
	if (bigInt->byteCount == 0) return;
	if (bigInt->byteCount == 1) {
		freeBigInt(bigInt);
		return;
	}
	bigInt->byteCount--;
	byte* newMem = (byte*)malloc(bigInt->byteCount);
	memcpy_s(newMem, bigInt->byteCount, bigInt->bytes, bigInt->byteCount);
	free(bigInt->bytes);
	bigInt->bytes = newMem;
}

void reduceSignExcessFromLastByte(BigInt* bigInt) {
	if (!bigInt->isHasSign) return;
	uint8_t signExcessBitLen = 0;
	while (signExcessBitLen < 8 && readBit(bigInt->bytes[bigInt->byteCount - 1], 7 - signExcessBitLen)==1)
		signExcessBitLen++;
	for (int i = 7; i >= 8 - signExcessBitLen; i--) {
		setBit(&bigInt->bytes[bigInt->byteCount - 1], i, 0);
	}
}

void reduceSignExcessBytes(BigInt* i) {
	byte signExcess = i->isHasSign ? 0b11111111 : 0b00000000;
	while (i->byteCount > 0 && i->bytes[i->byteCount - 1] == signExcess) {
		removeLastByteFromBigInt(i);
	}
	reduceSignExcessFromLastByte(i);
}

BigInt operator ~ (BigInt a) {
	BigInt tmpA = a;
	return a;
}