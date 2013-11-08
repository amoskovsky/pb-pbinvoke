#include "../stdafx.h"

#include "Int64Type.h"
#include "../DynCallback.h"



void Int64Type::executeMethod(ExecutionContext *context, DWORD methodAddr, BYTE * retBuf, BYTE *paramBuf, int paramBufSize, bool isStdCall)
{
	DWORD *argv = (DWORD *)paramBuf;
	int argc = paramBufSize / sizeof(DWORD);
	*(__int64*)retBuf = runDynamicInt64Method(methodAddr, argv, argc, isStdCall);

}
