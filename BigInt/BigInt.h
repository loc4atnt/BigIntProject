#ifndef _BigInt_H_
#define _BigInt_H_

#include <stdint.h>

struct BigInt {
	unsigned char* bytes;
	uint16_t byteCount;
	bool isHasSign;
};


#endif