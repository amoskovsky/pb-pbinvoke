#pragma once

#include "RawType.h"

class BoolType: public RawType {
public:
	BoolType() : RawType("bool", sizeof(bool)) {} 
	virtual int getPBTypeClass() { return TC_BOOL_TYPE; }
};
