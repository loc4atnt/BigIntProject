#ifndef _BigInt_H_
#define _BigInt_H_

#include "ByteUtil.h"
#include <stdint.h>

struct BigInt {
	byte* bytes;
	uint16_t byteCount;
	bool isHasSign;
};

#endif