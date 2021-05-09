#include "BigInt.h"
#include <malloc.h>
#include <string.h>
#include <math.h>

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
	short maxByte = max(a.byteCount, b.byteCount);
	if (a.isHasSign == b.isHasSign) {
		res.byteCount = maxByte + 1;
	}
	else res.byteCount = maxByte;
	res.bytes = (byte*)malloc(res.byteCount);	
	BigInt* p = &a;
	fillLastByteWithSignExcess(p);
	p = &b;
	fillLastByteWithSignExcess(p);
	if (a.byteCount != b.byteCount) {
		p = (a.byteCount > b.byteCount) ? &b : &a;
		addSignExcessBytes(p, (byte)fabs(a.byteCount - b.byteCount));
	}	
	res.bytes[0] = a.bytes[0] + b.bytes[0];
	bool bitOverflow = (a.bytes[0] + b.bytes[0] > 255) ? 1 : 0;
	
	for (int i = 1; i < maxByte; i++) {
		res.bytes[i] = a.bytes[i] + b.bytes[i] + (bitOverflow ? 1 : 0);
		if ((a.bytes[i] + b.bytes[i] + (bitOverflow ? 1 : 0)) > 255)  bitOverflow = 1;
		else bitOverflow = 0;
	}
	p = &res;
	if (a.isHasSign == b.isHasSign) {
		res.isHasSign = a.isHasSign;
		
		if (bitOverflow) {
			res.bytes[res.byteCount - 1] = 1;
			fillLastByteWithSignExcess(p);
		}
		reduceSignExcessBytes(p);
	}	
	else {
		if (a.byteCount == b.byteCount) {
			res.isHasSign = (!bitOverflow) ? 1 : 0;
		}
		else {
			p = (a.byteCount > b.byteCount) ? &a : &b;
			res.isHasSign = p->isHasSign;
		}
	}
	p = &a;
	reduceSignExcessBytes(p);
	p = &b;
	reduceSignExcessBytes(p);
	reduceSignExcessBytes(&res);
	return res;
}

BigInt oppositeNum(BigInt a)
{
	if (a == 0) return a;
	BigInt res = ~(a);
#ifdef DEBUG
	printf("oppositeNum - 1: %s\n", bigIntToBinStr(&res));
	printf("R: %d - %d - %d\n\n", res.isHasSign, res.byteCount, res.bytes[0]);
#endif
	res = res + assignValue(1);
#ifdef DEBUG
	printf("oppositeNum - 2: %s\n", bigIntToBinStr(&res));
#endif
	return res;
}

BigInt operator-(BigInt a, BigInt b) {
	BigInt res;
#ifdef DEBUG
	BigInt tmp = oppositeNum(b);
	printf("operator- - 1: %s\n", bigIntToBinStr(&a));
	printf("operator- - 2: %s\n", bigIntToBinStr(&tmp));
#endif
	res = a + oppositeNum(b);
	return res;
}

bool isOddBigInt(BigInt* bI) {
	if (bI->byteCount == 0) return false;
	return (bI->bytes[0] & 1);
}
bool isEvenBigInt(BigInt* bI) {
	return !isOddBigInt(bI);
}

void swap(BigInt* a, BigInt* b) {
	byte* tmpByte = a->bytes;
	uint16_t tmpByteCount = a->byteCount;
	bool tmpIsHasSign = a->isHasSign;

	a->bytes = b->bytes;
	a->byteCount = b->byteCount;
	a->isHasSign = b->isHasSign;

	b->bytes = tmpByte;
	b->byteCount = tmpByteCount;
	b->isHasSign = tmpIsHasSign;
}

BigInt operator * (BigInt a, BigInt b) {
	BigInt res = assignValue(0);
	bool isResNegative = (a.isHasSign == b.isHasSign);//a, b cung dau thi res duong, khac dau thi res am
	a = abs(a);
	b = abs(b);
	if (a < b) { // So hang thu 2 nen la so hang nho hon
		swap(&a, &b);
	}

	while (b > 0) {
		if (isOddBigInt(&b)) {
			res = res + a;
		}
		a = a << 1;// a*=2
		b = b >> 1;// b/=2;
	}
	if (isResNegative) return oppositeNum(res);
	return res;
}

BigInt operator / (BigInt a, BigInt b) {
	if (b == 0) return BigInt();

	BigInt q = assignValue(0);
	bool isResNegative = (a.isHasSign == b.isHasSign);//a, b cung dau thi res duong, khac dau thi res am
	a = abs(a);
	b = abs(b);

	BigInt componentQ;
	BigInt tmpB;
	while (a >= b) {
		componentQ = assignValue(1);
		tmpB = b;
		while ((tmpB = (tmpB << 1)) <= a) {
			componentQ = componentQ << 1;
		}
		tmpB = tmpB >> 1;// tra lai 1 lan dich bit khien dieu kien cua while tren sai
		q = q + componentQ;
		a = a - tmpB;
	}

	if (isResNegative) return oppositeNum(q);
	return q;
}

BigInt operator % (BigInt a, BigInt b) {
	if (b == 0) return BigInt();
	
	BigInt q = assignValue(0);
	bool isResNegative = (a.isHasSign == b.isHasSign);//a, b cung dau thi res duong, khac dau thi res am
	a = abs(a);
	b = abs(b);

	BigInt componentQ;
	BigInt tmpB;
	while (a >= b) {
		componentQ = assignValue(1);
		tmpB = b;
		while ((tmpB = (tmpB << 1)) <= a) {
			componentQ = componentQ << 1;
		}
		tmpB = tmpB >> 1;// tra lai 1 lan dich bit khien dieu kien cua while tren sai
		q = q + componentQ;
		a = a - tmpB;
	}

	if (isResNegative) return oppositeNum(a);
	return a;
}

bool isNonZero(BigInt z) {
	return z != 0;
}

BigInt decStrToBigInt(const char* decStr)
{
	BigInt res = assignValue(0);
	BigInt dec = assignValue(10);
	BigInt idx = assignValue(1);
	BigInt num = assignValue(0);
	short decStrLen = strlen(decStr);
	num.bytes[0] = decStr[decStrLen - 1] - '0';
	res = res + num * idx;
	for (int i = decStrLen-2; i >= 0 ; i--) {
		num.bytes[0] = decStr[i] - '0';
		idx = idx * dec;
		res = res + num * idx;
	}
	return res;
}

char* bigIntToDecStr(BigInt *b)
{
	byte i = b->byteCount * 24 / 10 + 1;
	char* res =  (char*)malloc(i);	
	i = { 0 };
	BigInt dec = assignValue(10),z1,z2=(*b);
	do
	{
		z1 = z2 % dec;    
		z2 = z2 / dec; 
		res[i] = z1.bytes[0] + '0';
		i--;
	} while (isNonZero(z2));	
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
	if ((isNegative = (iVal < 0))) {
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

uint16_t getBitLen(BigInt* bigInt) {
	uint16_t bitLen = (bigInt->byteCount * 8);
	for (int i = 7; i >= 0; i--) {
		if (readBit(bigInt->bytes[bigInt->byteCount - 1], i) == 1) break;
		bitLen--;
	}
	return bitLen;
}

BigInt operator << (BigInt a, int steps) {
	if (steps == 0) return a;
	if (steps < 0) return (a >> (-steps));

	BigInt res = a;
	uint16_t movingBitLen = getBitLen(&a);
	addSignExcessBytes(&res, (steps/8) + 1);
	for (int i = movingBitLen+steps-1; i >= steps; i--) {
		setBit(&res, i, readBit(&res, i - steps));
	}
	for (int i = 0; i < steps; i++) {
		setBit(&res, i, 0);
	}
	reduceSignExcessBytes(&res);
	return res;
}

BigInt AndOrXor(BigInt a, BigInt b, byte(*calFunc)(byte b1, byte b2)) {
	BigInt res = a;
	int sharedByteCount = max(a.byteCount, b.byteCount) + 1;
	addSignExcessBytes(&res, sharedByteCount - res.byteCount);
	addSignExcessBytes(&a, sharedByteCount - a.byteCount);
	addSignExcessBytes(&b, sharedByteCount - b.byteCount);
#ifdef DEBUG
	printf("debugandorxor\n");
	printf("%s\n", bigIntToBinStr(&res));
	printf("%s\n", bigIntToBinStr(&a));
	printf("%s\n", bigIntToBinStr(&b));
#endif
	for (int i = 0; i < sharedByteCount; i++) {
		res.bytes[i] = calFunc(a.bytes[i], b.bytes[i]);
	}
	res.isHasSign = readBit(res.bytes[res.byteCount - 1], 7);
	reduceSignExcessBytes(&res);
	return res;
}

BigInt operator & (BigInt a, BigInt b) {
	return AndOrXor(a, b, AndByte);
}

BigInt operator ^ (BigInt a, BigInt b) {
	return AndOrXor(a, b, XorByte);
}

BigInt operator | (BigInt a, BigInt b) {
	return AndOrXor(a, b, OrByte);
}

bool operator > (BigInt a, int b) {
	// Xu ly de quy
	// Truong hop co ban b == 0 HOAC a == 0
	if (a == 0) {
		return 0 > b;
	}
	if (b == 0) {
		return (a.isHasSign == 0); // && a!= 0 <=> a duong
	}

	BigInt tmpBigIntB = assignValue(b);
	return a > tmpBigIntB;
}

bool operator > (int a, BigInt b) {
	return b < a;
}

bool operator > (BigInt a, BigInt b) {
	if (a.isHasSign != b.isHasSign) return (a.isHasSign == false && b.isHasSign == true);// a khong am, b am thi return true
	
	// Xu ly de quy
	// Truong hop co ban: a == 0 HOAC b == 0
	if (a == 0) return (0 > b);
	if (b == 0) return (a > 0);
	BigInt delta = a - b;
	return delta>0;
}

bool operator < (BigInt a, int b) {
	// Xu ly de quy
	// Truong hop co ban b == 0 HOAC a == 0
	if (a == 0) {
		return 0 < b;
	}
	if (b == 0) {
		return (a.isHasSign == 1); // && a!= 0 <=> a am
	}

	BigInt tmpBigIntB = assignValue(b);
	return a < tmpBigIntB;
}

bool operator < (int a, BigInt b) {
	return b > a;
}

bool operator < (BigInt a, BigInt b) {
	if (a.isHasSign != b.isHasSign) return (a.isHasSign == true && b.isHasSign == false);// a am, b khong am thi return true

	// Xu ly de quy
	// Truong hop co ban: a == 0 HOAC b == 0
	if (a == 0) return (0 < b);
	if (b == 0) return (a < 0);
	BigInt delta = a - b;
	return delta < 0;
}

bool operator == (BigInt a, BigInt b) {
	//if (isNullBigInt(a) || isNullBigInt(b)) return false;
	//if (a.isHasSign != b.isHasSign) return false;
	//uint16_t compareByteAmount = max(a.byteCount, b.byteCount);
	//addSignExcessBytes(&a, compareByteAmount - a.byteCount);
	//addSignExcessBytes(&b, compareByteAmount - b.byteCount);
	//for (int i = 0; i < compareByteAmount; i++) {
	//	if (a.bytes[i] != b.bytes[i]) return false;
	//}
	//return true;

	if (isNullBigInt(a) || isNullBigInt(b)) return false;
	if (a.isHasSign != b.isHasSign) return false;

	BigInt delta = a - b;
	return delta == 0;
}

bool operator == (BigInt a, int b) {
	// Xu ly de quy
	// Truong hop co ban b == 0
	if (b == 0) {
		reduceSignExcessBytes(&a);
		return (a.byteCount == 1) && (a.bytes[0] == 0) && (a.isHasSign == false);
	}

	BigInt bigIntB = assignValue(b);
	return a == bigIntB;
}

bool operator != (BigInt a, int b) {
	return !(a == b);
}

bool isNullBigInt(BigInt& a) {
	return a.byteCount == 0 || a.bytes == NULL;
}

BigInt abs(BigInt i) {
	if (i.isHasSign == true) return oppositeNum(i);
	return i;
}

BigInt min(BigInt a, BigInt b) {
	return (a < b) ? a : b;
}

BigInt max(BigInt a, BigInt b) {
	return (a > b) ? a : b;
}

bool operator >= (BigInt a, BigInt b) {
	return (a == b) || (a > b);
}

bool operator <= (BigInt a, BigInt b) {
	return (a == b) || (a < b);
}