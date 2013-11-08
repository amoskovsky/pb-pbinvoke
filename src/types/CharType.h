#pragma once

#include "PointerType.h"
#include "../Runtime.h"

class CharType: public RawType {
protected:
	CharType(const string &typeSignature, size_t sizeOf);
public:
	virtual int getPBTypeClass() { return TC_CHAR_TYPE; };
	//inline void setPBUnicode(bool isPBUnicode) { m_isPBUnicode = isPBUnicode;}
protected:
	//bool m_isPBUnicode;
	string TypeName;
	string StringFormat;
};

class ACharType: public CharType {
protected:
	typedef char type_t;
	typedef wchar_t inv_type_t;
	enum {CHARSET = DC_ANSI, INV_CHARSET = DC_UNICODE };
	inline size_t tstrlen(const type_t* s) { return strlen(s);}
	inline size_t tstrnlen(const type_t* s, size_t n) { return strnlen(s, n);}
	inline size_t strlenStoT(ExecutionContext* ctx, const type_t *srcString, int strLen = -1) {
		return ctx->strlenAtoW(srcString, strLen);
	}
	inline size_t strlenTtoS(ExecutionContext* ctx, const inv_type_t *srcString, int strLen = -1) {
		return ctx->strlenWtoA(srcString, strLen);
	}
	inline inv_type_t* convertStoT(ExecutionContext* ctx, const type_t *srcString, int strLen = -1, inv_type_t *buf = NULL) {
		return ctx->convertAtoW(srcString, strLen, buf);
	}
	inline type_t* convertTtoS(ExecutionContext* ctx, const inv_type_t *srcString, int strLen = -1, type_t *buf = NULL) {
		return ctx->convertWtoA(srcString, strLen, buf);
	}
public:
	
	ACharType() : CharType("char", sizeof(char)) { TypeName = "ACharType"; StringFormat = "s"; }
	virtual void setArrayData(ExecutionContext *context, BYTE * &cBuf, BYTE *pbBuf, int strLen, DataCharset charset);
	virtual void getArrayData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &strLen, DataCharset charset);
	virtual size_t strLen(ExecutionContext *context, BYTE * buf, int arrayLen, DataCharset charset);
};
class WCharType: public CharType {
protected:
	typedef wchar_t type_t;
	typedef char inv_type_t;
	enum {CHARSET = DC_UNICODE, INV_CHARSET = DC_ANSI };
	inline size_t tstrlen(const type_t* s) { return wcslen(s);}
	inline size_t tstrnlen(const type_t* s, size_t n) { return wcsnlen(s, n);}
	inline size_t strlenStoT(ExecutionContext* ctx, const type_t *srcString, int strLen = -1) {
		return ctx->strlenWtoA(srcString, strLen);
	}
	inline size_t strlenTtoS(ExecutionContext* ctx, const inv_type_t *srcString, int strLen = -1) {
		return ctx->strlenAtoW(srcString, strLen);
	}
	inline inv_type_t* convertStoT(ExecutionContext* ctx, const type_t *srcString, int strLen = -1, inv_type_t *buf = NULL) {
		return ctx->convertWtoA(srcString, strLen, buf);
	}
	inline type_t* convertTtoS(ExecutionContext* ctx, const inv_type_t *srcString, int strLen = -1, type_t *buf = NULL) {
		return ctx->convertAtoW(srcString, strLen, buf);
	}

public:
	WCharType() : CharType("wchar_t", sizeof(wchar_t)) { TypeName = "WCharType"; StringFormat = "S"; }
	virtual void setArrayData(ExecutionContext *context, BYTE * &cBuf, BYTE *pbBuf, int strLen, DataCharset charset);
	virtual void getArrayData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &strLen, DataCharset charset);
	virtual size_t strLen(ExecutionContext *context, BYTE * buf, int arrayLen, DataCharset charset);
};


class TACharType: public ACharType {
public:
	virtual string getTypeSignature(const string& pointerSig = "") { return "tchar_t" + pointerSig;}
	virtual bool hasTChar() { return true; }
};

class TWCharType: public WCharType {
public:
	virtual string getTypeSignature(const string& pointerSig = "") { return "tchar_t" + pointerSig;}
	virtual bool hasTChar() { return true; }
};

