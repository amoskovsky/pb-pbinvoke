#pragma once

#include "RawType.h"

class PointerType : public RawType  
{
public:
	PointerType(Type &dataType, const string& ptrSig = "*", size_t sizeOf = sizeof(BYTE*));
	virtual int getPBTypeClass() { return TC_PTR_TYPE; }
	Type& getDataType() { return m_dataType; }
	virtual bool hasTChar() { dprintf("PointerType->hasTChar=%i\n", m_dataType->hasTChar()); return m_dataType->hasTChar(); }
protected:
	Type m_dataType;
};

