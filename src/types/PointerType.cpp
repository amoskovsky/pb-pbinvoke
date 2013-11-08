#include "../stdafx.h"

#include "PointerType.h"

#include "../PBException.h"

PointerType::PointerType(Type &dataType, const string& ptrSig, size_t sizeOf)
	: RawType(dataType->getTypeSignature(ptrSig), sizeOf)
	, m_dataType(dataType)
{
	dprintf("PointerType, type=%p, sig=%s\n", dataType.get(), dataType->getTypeSignature("*").c_str());	
}


BOOL PBCALL pt_getMetaData(PointerType* ptrType, BaseType* &dataType, int &dataTypeClass)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		dataType = ptrType->getDataType().get();
		if (dataType != NULL)
			dataTypeClass = dataType->getPBTypeClass();
		return true;
	}
	catch (...) {
	}
	return false;
}
