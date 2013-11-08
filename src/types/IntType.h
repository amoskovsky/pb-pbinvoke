#pragma once

#include "RawType.h"

class IntType: public RawType {
public:
	IntType() : RawType("int", sizeof(int)) {} 
	virtual int getPBTypeClass() { return TC_INT_TYPE; }
};

class THandleType: public RawType {
public:
	THandleType() : RawType("__thandle", sizeof(int)) {} 
	virtual int getPBTypeClass() { return TC_INT_TYPE; }
	virtual bool hasTChar() { return true; }
};
