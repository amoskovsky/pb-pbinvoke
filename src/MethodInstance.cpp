#include "StdAfx.h"

#include "MethodInstance.h"
#include "Runtime.h"
#include "PBException.h"

//typedef struct __declspec(align(16)) S {int a;} S;
//typedef enum U { AA } U;
//int U();

void PBCALL mi_destroy(MethodInstance *methodInstance) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		dprintf("mi_destroy methodInstance=%p\n", methodInstance);
		if (methodInstance != NULL)
			delete methodInstance;
		dprintf("mi_destroy OK\n");
	}
	catch(...) {
		dprintf("mi_destroy failed\n");
	}
}

DWORD PBCALL mi_getMethodAddr(MethodInstance* methodInstance) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		if (methodInstance != NULL)
			return methodInstance->getMethodAddr();
	}
	catch(...) {
	}
	return NULL;
}

MethodType* PBCALL mi_getMethodType(MethodInstance* methodInstance) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		if (methodInstance != NULL)
			return methodInstance->getMethodType();
	}
	catch(...) {
	}
	return NULL;
}
