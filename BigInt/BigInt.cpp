#include "BigInt.h"
#include <malloc.h>
#include <string.h>

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

void freeBigInt(BigInt bigI) {
	free(bigI.bytes);
}

BigInt operator+(BigInt a, BigInt b)
{
	BigInt res;
	res.byteCount = max(a.byteCount, b.byteCount) + 1;
	res.bytes = (byte*)malloc(res.byteCount);
	res.bytes[0] = a.bytes[0] + b.bytes[0];
	bool bitOverflow = (a.bytes[0] + b.bytes[0] > 255) ? 1 : 0;
	int i = 1;
	for (i; i < min(a.byteCount, b.byteCount); i++) {
		res.bytes[i] = a.bytes[i] + b.bytes[i] + (bitOverflow ? 1 : 0);
		if ((a.bytes[i] + b.bytes[i] + (bitOverflow ? 1 : 0)) > 255)  bitOverflow = 1;
		else bitOverflow = 0;
	}
	BigInt* p = (a.byteCount >= b.byteCount) ? &a : &b;
	for (i; i < res.byteCount-1; i++) {
		res.bytes[i] = p->bytes[i] + (bitOverflow ? 1 : 0);
		if ((p->bytes[i] + (bitOverflow ? 1 : 0)) > 255)  bitOverflow = 1;
		else bitOverflow = 0;
	}
	res.bytes[res.byteCount-1] = bitOverflow ? 1 : 0;
	//deleteByteZero(res);
	if (res.bytes[res.byteCount-1] == 0) {
		res.byteCount--;
		res.bytes = (byte*)realloc(res.bytes,res.byteCount);
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
