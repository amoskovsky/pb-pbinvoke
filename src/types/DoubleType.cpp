#include "../stdafx.h"

#include "DoubleType.h"
#include "../DynCallback.h"



void DoubleType::executeMethod(ExecutionContext *context, DWORD methodAddr, BYTE * retBuf, BYTE *paramBuf, int paramBufSize, bool isStdCall)
{
	DWORD *argv = (DWORD *)paramBuf;
	int argc = paramBufSize / sizeof(DWORD);
	*(double*)retBuf = runDynamicDoubleMethod(methodAddr, argv, argc, isStdCall);

}
