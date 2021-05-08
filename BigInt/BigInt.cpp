#include "BigInt.h"

BigInt binStrToBigInt(const char* binStr) {
	BigInt bigI;
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

char* bigIntToBinStr(BigInt *b) {
	if (b->byteCount == 0) return NULL;

	char* tmpBinStr = byteToBinStr(b->bytes[b->byteCount - 1]);
	int binStrBuffLen = strlen(tmpBinStr) + 1;
	char* binStr = (char*)malloc(binStrBuffLen);
	strcpy_s(binStr, binStrBuffLen, tmpBinStr);

	for (int i = b->byteCount - 2; i >= 0; i--) {
		free(tmpBinStr);
		tmpBinStr = byteToBinStr(b->bytes[i], true);
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

BigInt oppositeNum(BigInt a)
{
	BigInt res = ~(a);
	res = res + assignValue(1);
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
		free(bigInt->bytes);
		bigInt->bytes = NULL;
		bigInt->byteCount = 0;
		bigInt->isHasSign = false;
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

	// xu ly rut gon dau cho so am (bit dau = 1)
	uint8_t signExcessBitLen = 0;
	while (signExcessBitLen < 7 && readBit(bigInt->bytes[bigInt->byteCount - 1], 7 - signExcessBitLen)==1
		&& readBit(bigInt->bytes[bigInt->byteCount - 1], 7 - signExcessBitLen - 1) == 1)
		signExcessBitLen++;
	for (int i = 7; i >= 8 - signExcessBitLen; i--) {
		setBit(&bigInt->bytes[bigInt->byteCount - 1], i, 0);
	}
}

void reduceSignExcessBytes(BigInt* i) {
	byte signExcess = i->isHasSign ? 0b11111111 : 0b00000000;
	while (i->byteCount > 1 && i->bytes[i->byteCount - 1] == signExcess
		&& (readBit(i->bytes[i->byteCount - 2], 7) == i->isHasSign) ) {
		removeLastByteFromBigInt(i);
	}
	reduceSignExcessFromLastByte(i);
}

BigInt operator ~ (BigInt a) {
	BigInt notA = a;
	addSignExcessBytes(&notA, 1);
	for (int i = 0; i < notA.byteCount; i++) {
		notA.bytes[i] = ~notA.bytes[i];
	}
	notA.isHasSign = !notA.isHasSign;
	reduceSignExcessBytes(&notA);
	return notA;
}

BigInt assignValue(int iVal) {
	BigInt bI;
	if (iVal == 0) {
		addByteToBigInt(&bI, 0);
		return bI;
	}

	bool isNegative;
	if ( (isNegative=(iVal < 0)) ) {
		iVal = -iVal;
	}
	
	byte tmpDigit;
	while (iVal > 0) {
		tmpDigit = iVal % 256;
		iVal /= 256;
		addByteToBigInt(&bI, tmpDigit);
	}
	if (isNegative) bI = oppositeNum(bI);
	return bI;
}

bool readBit(BigInt *a, uint16_t idx) {
	fillLastByteWithSignExcess(a);
	uint16_t byteIdx = idx / 8;
	uint8_t bitInByteIdx = idx % 8;
	if (byteIdx >= a->byteCount) return a->isHasSign;
	return readBit(a->bytes[byteIdx], bitInByteIdx);
}

void setBit(BigInt *a, uint16_t idx, bool bit) {
	uint16_t byteIdx = idx / 8;
	uint8_t bitInByteIdx = idx % 8;
	if (byteIdx >= (a->byteCount - 1)) {
		addSignExcessBytes(a, byteIdx - (a->byteCount - 1) + 1);
	}

	setBit(&(a->bytes[byteIdx]), bitInByteIdx, bit);
	reduceSignExcessBytes(a);
}

BigInt operator >> (BigInt a, int steps) {
	if (steps == 0) return a;
	if (steps < 0) return (a<<(-steps));

	BigInt res = a;
	uint16_t movingBitLen = res.byteCount * 8;
	for (int i = 0; i < movingBitLen; i++) {
		setBit(&res, i, readBit(&res, i + steps));
	}

	reduceSignExcessFromLastByte(&res);
	return res;
}

BigInt operator << (BigInt a, int steps) {
	if (steps == 0) return a;
	if (steps < 0) return (a >> (-steps));

	BigInt res = a;
	return res;
}