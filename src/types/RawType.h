
#pragma once

#include "BaseType.h"

// Generic plain C type 
class RawType: public BaseType
{
private:
protected:
	RawType(const string &typeSignature, size_t sizeOf = sizeof(DWORD));
	size_t m_sizeOf;
	string m_typeSignature;

	void setSizeOf(size_t sizeOf) { m_sizeOf = sizeOf; }
public:
	virtual int getSizeOf() { return m_sizeOf; }
	//virtual void setData(ExecutionContext *context, BYTE * cBuf, BYTE * pbBuf);
	//virtual void getData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf);
	virtual void executeMethod(ExecutionContext *context, DWORD methodAddr, BYTE * retBuf, BYTE *paramBuf, int paramBufSize, bool isStdCall);
	virtual int getItemOffset(int itemIndex) {return 0;}
	virtual void allocateBuffer(ExecutionContext *context, BYTE * &buf, int itemCount = 1);
	virtual string getTypeSignature(const string& pointerSig = "");
	virtual void setZeroData(ExecutionContext *context, BYTE * buf);
	virtual int getExplicitAlign() { return 0; }
	virtual int getAlign() { return getSizeOf(); }
	// unimplemented (throw)
	virtual void getArrayData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &arrayLen, DataCharset charset);
	virtual void setArrayData(ExecutionContext *context, BYTE * &cBuf, BYTE *pbBuf, int arrayLen, DataCharset charset);
	virtual size_t strLen(ExecutionContext *context, BYTE * buf, int arrayLen, DataCharset charset) { return 0;}
};
