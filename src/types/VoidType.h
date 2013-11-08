#pragma once

#include "RawType.h"

class VoidType: public RawType {
public:
	VoidType() : RawType("void", sizeof(char)) {} 
	virtual int getPBTypeClass() { return TC_VOID_TYPE; }
};
