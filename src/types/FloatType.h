#pragma once

#include "RawType.h"

class FloatType: public RawType {
public:
	FloatType() : RawType("float", sizeof(float)) {} 
	virtual int getPBTypeClass() { return TC_FLOAT_TYPE; }
	virtual void executeMethod(ExecutionContext *context, DWORD methodAddr, BYTE * retBuf, BYTE *paramBuf, int paramBufSize, bool isStdCall);
};


