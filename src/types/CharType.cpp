#include "../stdafx.h"

#include "CharType.h"

#include "../Runtime.h"
#include "../PBException.h"

CharType::CharType(const string &typeSignature, size_t sizeOf)
	: RawType(typeSignature, sizeOf)
{
} 

template <class T = ACharType>
class Convertor: public T {
public:
	inline size_t strLen(ExecutionContext *context, BYTE * buf, int arrayLen, DataCharset charset);
	inline void setArrayData(ExecutionContext *context, BYTE * &cBuf, BYTE * pbBuf, int strLen, DataCharset charset);
	inline void getArrayData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &strLen, DataCharset charset);
};

template<class T>
inline size_t Convertor<T>::strLen(ExecutionContext *context, BYTE * buf, int arrayLen, DataCharset charset)
{
	size_t strLen = 0;
	if (buf != NULL) {
		if (charset == CHARSET) {
			strLen = tstrnlen((type_t*) buf, arrayLen);
		}
		else {
			strLen = strlenTtoS(context, (inv_type_t*)buf, arrayLen);
		}
	}
	return strLen;
}

template<class T>
inline void Convertor<T>::setArrayData(ExecutionContext *context, BYTE * &cBuf, BYTE * pbBuf, int strLen, DataCharset charset)
{
	dprintf("+%s::setArrayData cBuf=%p, strLen=%i, s='%s'\n", TypeName.c_str(), cBuf ,strLen, pbBuf);

	if (pbBuf == NULL)
		throw RuntimeException(context, TypeName+ "::setArrayData pbBuf == NULL");


	if (pbBuf != NULL) {
		if (charset == CHARSET || charset == DC_BINARY) {
			if (cBuf == NULL) {
				strLen += 1;
				allocateBuffer(context, cBuf, strLen);
			}
			memcpy(cBuf, pbBuf, strLen * sizeof(type_t));	
		}
		else {
			cBuf = (BYTE*)convertTtoS(context, (inv_type_t*)pbBuf, strLen, (type_t*)cBuf);
		}
	}

	dprintf("-%s::setArrayData\n", TypeName.c_str());
}

template<class T>
inline void Convertor<T>::getArrayData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &strLen, DataCharset charset)
{
	dprintf("+%s::getArrayData cBuf=%p, strLen=%i\n", TypeName.c_str(), cBuf ,strLen);
	if (cBuf == NULL)
		throw RuntimeException(context, TypeName + "::getArrayData cBuf == NULL");

	type_t *strValue = (type_t*)cBuf;
	dprintf("strLen=%i, strValue=%p, strValue='%s'\n", strLen, strValue, strValue);
	int len;
	if (charset == INV_CHARSET) {
		len = strlenStoT(context, strValue);
	}
	else if (charset == CHARSET) {
		len = tstrlen(strValue);
	}
	else { // DC_BINARY
		len = strLen;
	}
	if (strLen > 0) {
		if (len > strLen) // copy max strLen chars
			len = strLen; 
		dprintf("len=%i\n", len);
		if (charset == INV_CHARSET) {
			convertStoT(context, strValue, len, /*out*/(inv_type_t*)pbBuf);
		}
		else {
			memcpy(pbBuf, strValue, len * sizeof(type_t));
		}
	}
	strLen = len;
	dprintf("-%s::getArrayData\n", TypeName.c_str());

}

static Convertor<ACharType> convA;
static Convertor<WCharType> convW;

size_t ACharType::strLen(ExecutionContext *context, BYTE * buf, int arrayLen, DataCharset charset)
{
	return convA.strLen(context, buf, arrayLen, charset);
}

size_t WCharType::strLen(ExecutionContext *context, BYTE * buf, int arrayLen, DataCharset charset)
{
	return convW.strLen(context, buf, arrayLen, charset);
}


void ACharType::setArrayData(ExecutionContext *context, BYTE * &cBuf, BYTE * pbBuf, int strLen, DataCharset charset)
{
	convA.setArrayData(context, cBuf, pbBuf, strLen, charset);
}


void WCharType::setArrayData(ExecutionContext *context, BYTE * &cBuf, BYTE * pbBuf, int strLen, DataCharset charset)
{
	convW.setArrayData(context, cBuf, pbBuf, strLen, charset);
}


void ACharType::getArrayData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &strLen, DataCharset charset)
{
	convA.getArrayData(context, cBuf, pbBuf, strLen, charset);
}



void WCharType::getArrayData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &strLen, DataCharset charset)
{
	convW.getArrayData(context, cBuf, pbBuf, strLen, charset);
}


