#pragma once

#include "RawType.h"

class ShortType: public RawType {
public:
	ShortType() : RawType("short", sizeof(short)) {} 
	virtual int getPBTypeClass() { return TC_SHORT_TYPE; }
};
