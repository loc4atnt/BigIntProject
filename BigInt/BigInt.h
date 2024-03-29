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

BigInt binStrToBigInt(const char* binStr, bool isHasSign = false);
char* bigIntToBinStr(BigInt *b);
void addByteToBigInt(BigInt* bigInt, byte b);
void fillLastByteWithSignExcess(BigInt* i);
void addSignExcessBytes(BigInt* i, uint8_t amount);
void removeLastByteFromBigInt(BigInt* bigInt);
void reduceSignExcessBytes(BigInt* i);
void reduceSignExcessFromLastByte(BigInt* bigInt);
bool isNullBigInt(BigInt& a);
BigInt decStrToBigInt(const char* decStr);
char* bigIntToDecStr(BigInt *b);
int32_t getValue(BigInt n);

bool isOddBigInt(BigInt* bI);
bool isEvenBigInt(BigInt* bI);
BigInt assignValue(int val);
BigInt oppositeNum(BigInt a);
BigInt operator + (BigInt a, BigInt b);
BigInt operator - (BigInt a, BigInt b);
BigInt operator * (BigInt a, BigInt b);
BigInt operator / (BigInt a, BigInt b);
BigInt operator % (BigInt a, BigInt b);

void divAndMod(BigInt a, BigInt b, BigInt* qRes, BigInt* rRes);

BigInt AndOrXor(BigInt a, BigInt b, byte (*calFunc)(byte b1, byte b2));
BigInt operator ~ (BigInt a);
BigInt operator & (BigInt a, BigInt b);
BigInt operator ^ (BigInt a, BigInt b);
BigInt operator | (BigInt a, BigInt b);

BigInt operator >> (BigInt a, int steps);
BigInt operator << (BigInt a, int steps);

bool operator > (int a, BigInt b);
bool operator > (BigInt a, int b);
bool operator > (BigInt a, BigInt b);
bool operator >= (BigInt a, BigInt b);
bool operator < (int a, BigInt b);
bool operator < (BigInt a, int b);
bool operator < (BigInt a, BigInt b);
bool operator <= (BigInt a, BigInt b);
bool operator == (BigInt a, BigInt b);
bool operator == (BigInt a, int b);
bool operator != (BigInt a, BigInt b);
bool operator != (BigInt a, int b);

bool readBit(BigInt *a, uint16_t idx);
void setBit(BigInt *a, uint16_t idx, bool bit);
uint16_t getBitLen(BigInt* bigInt);
uint16_t getZeroBitSuffixLen(BigInt* i);

BigInt abs(BigInt i);
BigInt min(BigInt a, BigInt b);
BigInt max(BigInt a, BigInt b);
uint16_t digits(BigInt *i);
BigInt pow(BigInt a, BigInt e);
char* to_string(BigInt* i);
char* to_base58(BigInt i);
char* to_base10(BigInt *i);
char* to_base32Or64(BigInt *i, int8_t byteRoundingAmount, int8_t patternBitLen, char (*valueToBase)(uint8_t val));
char* to_base32(BigInt i);
char* to_base64(BigInt i);

static int PRIME_CHECKING_A[3] = { 2,7,61 };
BigInt powAndMod(BigInt a, BigInt e, BigInt m);
void primeDecomposeNum(BigInt n, int32_t* s, BigInt* d);
BigInt powOf2With(int32_t s);
bool checkSecondConditionOfComposite(BigInt aPowD, int32_t s, BigInt n);
bool is_prime(BigInt i);

#endif
