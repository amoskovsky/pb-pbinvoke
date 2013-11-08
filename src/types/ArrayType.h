#pragma once

#include "PointerType.h"

class ArrayType : public PointerType  
{
public:
	ArrayType(Type &dataType, size_t bound);
	virtual int getPBTypeClass() { return TC_ARRAY_TYPE; }
	virtual int getAlign() { return m_dataType->getAlign(); }
protected:
	size_t m_bound;

};


