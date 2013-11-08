#pragma once

#include "PointerType.h"

//class StringType: public PointerType {
//protected:
//	StringType(const string &typeSignature);
//public:
//	virtual int getPBTypeClass();
//	//inline void setPBUnicode(bool isPBUnicode) { m_isPBUnicode = isPBUnicode;}
//protected:
//	//bool m_isPBUnicode;
//};
//
//class AStringType: public StringType {
//public:
//	AStringType() : StringType("char*") {}
//	virtual void setStringData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int strLen, DataCharset charset);
//	virtual void getStringData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &strLen, DataCharset charset);
//};
//class WStringType: public StringType {
//public:
//	WStringType() : StringType("wchar_t*") {}
//	virtual void setStringData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int strLen, DataCharset charset);
//	virtual void getStringData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &strLen, DataCharset charset);
//};
