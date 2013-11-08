#include "../stdafx.h"

#include "ArrayType.h"

#include "../PBException.h"

ArrayType::ArrayType(Type &dataType, size_t bound)
	: PointerType(dataType, "[" + string(CUtil::toString(bound)) + "]", dataType->getSizeOf() * bound)
	, m_bound(bound)
{
	//if (m_bound == 0)
	//	throw RuntimeException("Internal error: ArrayType::ArrayType expect non-zero array bound");
}




