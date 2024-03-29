#include "BigInt.h"
#include <malloc.h>
#include <string.h>
#include <math.h>

BigInt binStrToBigInt(const char* binStr, bool isHasSign) {
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

	if (isHasSign) { bigI.isHasSign = isHasSign; }

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
	fillLastByteWithSignExcess(&a);
	fillLastByteWithSignExcess(&b);
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
		res.isHasSign = a.isHasSign;
		
		if (bitOverflow) {
			res.bytes[res.byteCount - 1] = 1;
			fillLastByteWithSignExcess(&res);
		}
		else removeLastByteFromBigInt(&res);
	}	
	else {
		if (a.byteCount == b.byteCount) {
			res.isHasSign = (!bitOverflow) ? 1 : 0;
		}
		else {
			BigInt* p = (a.byteCount > b.byteCount) ? &a : &b;
			res.isHasSign = p->isHasSign;
		}
	}
	reduceSignExcessBytes(&res);
	reduceSignExcessBytes(&a);
	reduceSignExcessBytes(&b);
	return res;
}

BigInt oppositeNum(BigInt a)
{
	if (a == 0) return a;
	BigInt res = ~(a);
	res = res + assignValue(1);
	return res;
}

BigInt operator-(BigInt a, BigInt b) {
	BigInt res;
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
	bool isResNegative = (a.isHasSign != b.isHasSign);//a, b cung dau thi res duong, khac dau thi res am
	a = abs(a);
	b = abs(b);
	if (a < b) { // So hang thu 2 nen la so hang nho hon
		swap(&a, &b);
	}

	uint16_t tmp;
	while (b > 0) {
		if (isOddBigInt(&b)) {
			res = res + a;
			tmp = 1;
		}
		else {
			tmp = getZeroBitSuffixLen(&b);
		}
		a = a << tmp;// a*=(2^tmp);
		b = b >> tmp;// b/=(2^tmp);
	}
	if (isResNegative) return oppositeNum(res);
	return res;
}

BigInt operator / (BigInt a, BigInt b) {
	BigInt res;
	divAndMod(a, b, &res, NULL);
	return res;
}

BigInt operator % (BigInt a, BigInt b) {
	BigInt res;
	divAndMod(a, b, NULL, &res);
	return res;
}

BigInt decStrToBigInt(const char* decStr)
{
	BigInt res = assignValue(0);
	short decStrLen = strlen(decStr);
	if (decStrLen == 0) return res;
	BigInt dec = assignValue(10);
	BigInt idx = assignValue(1);
	BigInt num = assignValue(0);
	for (int i = decStrLen-1; i > 0 ; i--) {
		num.bytes[0] = decStr[i] - '0';
		res = res + num * idx;
		idx = idx * dec;
	}
	res.isHasSign = 0;
	if (decStr[0] == '-') {
		res = oppositeNum(res);
	}
	else {
		num.bytes[0] = decStr[0] - '0';
		res = res + num * idx;
	}
	return res;
}

char* bigIntToDecStr(BigInt* b)
{
	return to_base10(b);
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

bool operator != (BigInt a, BigInt b) {
	return !(a == b);
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

uint16_t digits(BigInt *i) {
	if (*i == 0) return 1;
	uint16_t cnt = 0;
	BigInt tmp =(*i), dec = assignValue(10);
	while (tmp != 0) {
		cnt++;
		tmp = tmp / dec;
	}
	return cnt;
}

BigInt pow(BigInt a, BigInt e) {
	if (e == 0) return assignValue(1);
	else if (e == 1) return a;
	else {
		BigInt n = pow(a, e >> 1);
		return (isOddBigInt(&e) ? n * n * a : n * n);
	}
}

int32_t getValue(BigInt n) {
	int32_t value = 0;
	BigInt nAbs = abs(n);

	uint8_t valueBytesCount = nAbs.byteCount > 4 ? 4 : nAbs.byteCount;
	for (uint8_t i = 0; i < valueBytesCount; i++) {
		value += nAbs.bytes[i] * ((int32_t)pow(256, i));
	}
	return n.isHasSign ? -value : value;
}

char* to_string(BigInt* i) {
	return bigIntToDecStr(i);
}

char* to_base32Or64(BigInt *i, int8_t byteRoundingAmount, int8_t patternBitLen, char (*valueToBase)(uint8_t val)) {
	char* baseStr = (char*)malloc(1); baseStr[0] = '\0';
	int baseStrLen = 0;
	uint8_t paddingByteAmount = (byteRoundingAmount - (i->byteCount % byteRoundingAmount)) % byteRoundingAmount;
	addSignExcessBytes(i, paddingByteAmount);

	bool isAddPadding = true;
	uint8_t bitIdxOfEncPattern = 0;
	byte encPattern = 0;
	for (int k = i->byteCount - 1; k >= 0; k--) {
		for (int j = 0; j < 8; j++) {
			setBit(&encPattern, bitIdxOfEncPattern, readBit(i->bytes[k], j));
			if (bitIdxOfEncPattern == (patternBitLen-1)) {
				if (isAddPadding) {
					if (encPattern == 0) {
						insertCharFrontStr(&baseStr, &baseStrLen, PADDING_BASE);
					}
					else {
						isAddPadding = false;
						insertCharFrontStr(&baseStr, &baseStrLen, valueToBase(encPattern));
					}
				}
				else {
					insertCharFrontStr(&baseStr, &baseStrLen, valueToBase(encPattern));
				}
				bitIdxOfEncPattern = 0;
				encPattern = 0;
			}
			else {
				bitIdxOfEncPattern++;
			}
		}
	}

	return baseStr;
}

char* to_base32(BigInt i) {
	return to_base32Or64(&i, 5, 5, valueToBase32Char);
}

char* to_base64(BigInt i) {
	return to_base32Or64(&i, 3, 6, valueToBase64Char);
}

char* to_base58(BigInt i) {
	i.isHasSign = false;

	BigInt baseNum = assignValue(58);
	BigInt r;
	char* base58Str = (char*)malloc(1); base58Str[0] = '\0';
	int base58StrLen = 0;

	do {
		divAndMod(i, baseNum, &i, &r);
		insertCharFrontStr(&base58Str, &base58StrLen, valueToBase58Char(getValue(r)));
	} while (i > 0);

	return base58Str;
}

void divAndMod(BigInt a, BigInt b, BigInt *qRes, BigInt *rRes) {
	if (b == 0) return;

	BigInt q = assignValue(0);
	bool isResNegative = (a.isHasSign != b.isHasSign);//a, b cung dau thi res duong, khac dau thi res am
	b = abs(b);
	a = abs(a);

	int32_t deltaBitLen;
	int32_t bBitLen = getBitLen(&b);
	while ((deltaBitLen = (getBitLen(&a) - bBitLen)) > 1) {
		deltaBitLen -= 1;
		q = q + (assignValue(1) << deltaBitLen);
		a = a - (b << deltaBitLen);
	}

	while (a >= b) {
		q = q + assignValue(1);
		a = a - b;
	}

	if (isResNegative) {
		if (qRes) (*qRes) = oppositeNum(q);
		if (rRes) (*rRes) = oppositeNum(a);
	}
	else {
		if (qRes) (*qRes) = q;
		if (rRes) (*rRes) = a;
	}
}

char* to_base10(BigInt *n) {
	bool sign = n->isHasSign;
	BigInt i = abs(*n);

	BigInt baseNum = assignValue(10);
	BigInt r;
	char* base10Str = (char*)malloc(1); base10Str[0] = '\0';
	int base10StrLen = 0;

	do {
		divAndMod(i, baseNum, &i, &r);
		insertCharFrontStr(&base10Str, &base10StrLen, '0'+getValue(r));
	} while (i > 0);
	if (sign == 1) {
		insertCharFrontStr(&base10Str, &base10StrLen, '-');
	}

	return base10Str;
}

///////////////
void primeDecomposeNum(BigInt n, int32_t *s, BigInt *d) {// n = (2^s)*d
	(*s) = 0;
	while (readBit(&n, 0) == 0) {
		(*s)++;
		n = n >> 1;
	}
	*d = n;
}

BigInt powOf2With(int32_t s) {
	if (s < 0) return BigInt();
	return (assignValue(1) << s);
}

bool checkSecondConditionOfComposite(BigInt aPowDModN, int32_t s, BigInt n) {
	BigInt _n = (n - assignValue(1));
	if (s < 0) s = 0;
	while (s >= 0) {
		if (aPowDModN == _n) return false;
		aPowDModN = (aPowDModN * aPowDModN) % n;
		s--;
	}
	return true;
}

// (a^e) mod m
BigInt powAndMod(BigInt a, BigInt e, BigInt m) {
	a = a % m;
	BigInt res = assignValue(1);
	while (e > 0) {
		if (isOddBigInt(&e)) res = (res * a) % m;
		a = (a * a) % m;
		e = e >> 1;
	}
	return res;
}

bool is_prime(BigInt n) {
	if (n.isHasSign || n < 2 || readBit(&n, 0) == 0) return false;
	if (n == 2 || n == 3 || n == 5) return true;

	int32_t s;
	BigInt d;
	BigInt aPowDModN;
	BigInt a;
	primeDecomposeNum(n - assignValue(1), &s, &d);

	for (int i = 0; i < 3; i++) {
		a = assignValue(PRIME_CHECKING_A[i]);
		if (a >= n) break;
		aPowDModN = powAndMod(a, d, n);
		if (aPowDModN != 1 && checkSecondConditionOfComposite(aPowDModN, s - 1, n)) return false;
	}

	return true;
}

uint16_t getZeroBitSuffixLen(BigInt* n) {
	if (n == 0) return 0;
	uint16_t len = 0;
	int i;
	for (i = 0; i < n->byteCount; i++) {
		if (n->bytes[i] == 0) len += 8;
		else break;
	}
	if (i < n->byteCount) {
		for (int j = 0; j < 8; j++) {
			if (readBit(n->bytes[i], j)) break;
			else len++;
		}
	}
	return len;
}
