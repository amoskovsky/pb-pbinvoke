#include "../stdafx.h"

#include "Structure.h"
#include "../PBException.h"

Structure::Structure(StructType *structType)
	: ParamStorage(structType->getItems())
{

}

Structure::~Structure()
{

}


Structure * PBCALL str_create(StructType *structType) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		return new Structure(structType);
	}
	catch (...) {
		return NULL;
	}
}


BOOL PBCALL str_getContextData(Structure *structure, BYTE* fieldAddr[], int fieldCount) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		for (int i = 0; i < fieldCount; i ++) {
			fieldAddr[i] = structure->getParamAddr(i);
		}
		return true;
	}
	catch (...) {
		return false;
	}
}
