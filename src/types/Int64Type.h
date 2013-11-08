#pragma once

#include "RawType.h"

class Int64Type: public RawType {
public:
	Int64Type() : RawType("__int64", sizeof(__int64)) {} 
	virtual int getPBTypeClass() { return TC_LONGLONG_TYPE; }
	virtual void executeMethod(ExecutionContext *context, DWORD methodAddr, BYTE * retBuf, BYTE *paramBuf, int paramBufSize, bool isStdCall);
};
