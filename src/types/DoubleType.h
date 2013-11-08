#pragma once

#include "RawType.h"

class DoubleType: public RawType {
public:
	DoubleType() : RawType("double", sizeof(double)) {} 
	virtual int getPBTypeClass() { return TC_DOUBLE_TYPE; }
	virtual void executeMethod(ExecutionContext *context, DWORD methodAddr, BYTE * retBuf, BYTE *paramBuf, int paramBufSize, bool isStdCall);
};

