#include "../stdafx.h"

#include "FloatType.h"
#include "../DynCallback.h"


void FloatType::executeMethod(ExecutionContext *context, DWORD methodAddr, BYTE * retBuf, BYTE *paramBuf, int paramBufSize, bool isStdCall)
{
	DWORD *argv = (DWORD *)paramBuf;
	int argc = paramBufSize / sizeof(DWORD);
	*(float*)retBuf = runDynamicFloatMethod(methodAddr, argv, argc, isStdCall);

}
