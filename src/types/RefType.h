#pragma once

#include "PointerType.h"

class RefType : public PointerType  
{
public:
	RefType(Type &dataType);
	virtual int getPBTypeClass() { return TC_REF_TYPE; }
};


