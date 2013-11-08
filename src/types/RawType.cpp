#include "../stdafx.h"

#include "RawType.h"
#include "../DynCallback.h"
#include "../Runtime.h"
#include "../PBException.h"


RawType::RawType(const string &typeSignature, size_t sizeOf) 
	: m_typeSignature(typeSignature)
	, m_sizeOf(sizeOf) 
{
}


//void RawType::setData(ExecutionContext *context, BYTE * cBuf, BYTE * pbBuf)
//{
//	dprintf("+RawType::setData cBuf=%p, pbBuf=%p\n", cBuf, pbBuf);
//	memcpy(cBuf, pbBuf, m_sizeOf);
//	dprintf("-RawType::setData\n");
//}
//
//
//void RawType::getData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf)
//{
//	memcpy(pbBuf, cBuf, m_sizeOf);
//}

void RawType::executeMethod(ExecutionContext *context, DWORD methodAddr, BYTE * retBuf, BYTE *paramBuf, int paramBufSize, bool isStdCall)
{
	dprintf("+RawType::executeMethod paramBuf=%p, paramBufSize=%i\n", paramBuf, paramBufSize);
	dprintf("+RawType::executeMethod paramBuf data=%p %p\n", ((DWORD *)paramBuf)[0], ((DWORD *)paramBuf)[1]);
	DWORD *argv = (DWORD *)paramBuf;
	int argc = paramBufSize / sizeof(DWORD);
	*(DWORD*)retBuf = runDynamicMethod(methodAddr, argv, argc, isStdCall);
	dprintf("-RawType::executeMethod");
}

void RawType::getArrayData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &arrayLen, DataCharset charset)
{
	//throw RuntimeException(context, "getStringData() is not implemented for this type");
	memcpy(pbBuf, cBuf, getSizeOf() * arrayLen);

}

void RawType::setArrayData(ExecutionContext *context, BYTE * &cBuf, BYTE *pbBuf, int arrayLen, DataCharset charset)
{
	if (cBuf == NULL) {
		allocateBuffer(context, /*out*/ cBuf, arrayLen);
	}
	memcpy(cBuf, pbBuf, getSizeOf() * arrayLen);
}

void RawType::allocateBuffer(ExecutionContext *context, BYTE * &buf, int itemCount)
{
	int byteSize = getSizeOf() * itemCount;
	buf = context->allocateBuffer(byteSize, getAlign());
	if (buf != NULL)
		memset(buf, 0, byteSize);
}

void RawType::setZeroData(ExecutionContext *context, BYTE * buf)
{
	if (buf != NULL)
		memset(buf, 0, m_sizeOf);
}


string RawType::getTypeSignature(const string& pointerSig)
{
	if (pointerSig != "")
		return m_typeSignature + pointerSig;
	else
		return m_typeSignature;
}
