#include "../stdafx.h"

#include "StringType.h"

#include "../Runtime.h"
#include "../PBException.h"

//StringType::StringType(const string &typeSignature)
//	: PointerType(typeSignature)
//{
//} 
//
//
//int StringType::getPBTypeClass()
//{
//	return TC_STRING_TYPE;
//}
//
//
//
//void AStringType::setStringData(ExecutionContext *context, BYTE * cBuf, BYTE * pbBuf, int strLen, DataCharset charset)
//
//{
//	dprintf("\n+AStringType::setStringData str=[%s]\n", pbBuf);
//
//	// copy string value
//	if (pbBuf != NULL) {
//		if (charset == DC_ANSI || charset == DC_BINARY) {
//			BYTE *strValue = context->allocateBuffer((strLen + (charset == DC_BINARY ? 0 : 1)) * sizeof(char), __alignof(char));
//			memcpy(strValue, pbBuf, (strLen + (charset == DC_BINARY ? 0 : 1)) * sizeof(char));	
//			pbBuf = strValue;
//		}
//		else {
//			pbBuf = (BYTE *)context->convertWtoA((wchar_t*)pbBuf, strLen);
//		}
//	}
//
//	// copy ptr value
//	setData(context, cBuf, (BYTE*)&pbBuf);
//
//	dprintf("-AStringType::setStringData\n\n");
//}
//
//void AStringType::getStringData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &strLen, DataCharset charset)
//{
//	dprintf("+AStringType::getStringData\n");
//
//	char *strValue = *(char* *)cBuf;
//	if (strValue == NULL) {
//		strLen = -1;
//	}
//	else {
//		dprintf("strLen=%i, strValue=%p, strValue='%s'\n", strLen, strValue, strValue);
//		int len;
//		if (charset == DC_UNICODE) {
//			len = context->strlenAtoW(strValue);
//		}
//		else if (charset == DC_ANSI){
//			len = strlen(strValue);
//		}
//		else { // DC_BINARY
//			len = strLen;
//		}
//		if (strLen > 0) {
//			if (len > strLen) // copy max strLen chars
//				len = strLen; 
//			dprintf("len=%i\n", len);
//			if (charset == DC_UNICODE) {
//				context->convertAtoW(strValue, len, /*out*/(wchar_t*)pbBuf);
//			}
//			else {
//				memcpy(pbBuf, strValue, len * sizeof(char));
//			}
//		}
//		strLen = len;
//	}
//	dprintf("-AStringType::getStringData\n");
//}
//
//
//void WStringType::setStringData(ExecutionContext *context, BYTE * cBuf, BYTE * pbBuf, int strLen, DataCharset charset)
//
//{
//	dprintf("\n+WStringType::setStringData\n");
//
//	// copy string value
//	if (pbBuf != NULL) {
//		if (charset == DC_UNICODE || charset == DC_BINARY) {
//			BYTE *strValue = context->allocateBuffer((strLen + (charset == DC_BINARY ? 0 : 1)) * sizeof(wchar_t), __alignof(wchar_t));
//			memcpy(strValue, pbBuf, (strLen + (charset == DC_BINARY ? 0 : 1)) * sizeof(wchar_t));	
//			pbBuf = strValue;
//		}
//		else {
//			pbBuf = (BYTE *)context->convertAtoW((char*)pbBuf, strLen);
//		}
//	}
//
//	// copy ptr value
//	setData(context, cBuf, (BYTE*)&pbBuf);
//
//	dprintf("-WStringType::setStringData\n\n");
//}
//
//void WStringType::getStringData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &strLen, DataCharset charset)
//{
//	dprintf("+WStringType::getStringData\n");
//
//	wchar_t *strValue = *(wchar_t* *)cBuf;
//	if (strValue == NULL) {
//		strLen = -1;
//	}
//	else {
//		dprintf("strLen=%i, strValue=%p, strValue='%S'\n", strLen, strValue, strValue);
//		int len;
//		if (charset == DC_ANSI) {
//			len = context->strlenWtoA(strValue);
//		}
//		else if (charset == DC_UNICODE) {
//			len = wcslen(strValue);
//		}
//		else { // DC_BINARY
//			len = strLen;
//		}
//		if (strLen > 0) {
//			if (len > strLen) // copy max strLen chars
//				len = strLen; 
//			dprintf("len=%i\n", len);
//			if (charset == DC_ANSI) {
//				context->convertWtoA(strValue, len, /*out*/(char*)pbBuf);
//			}
//			else {
//				memcpy(pbBuf, strValue, len * sizeof(wchar_t));
//			}
//		}
//		strLen = len;
//	}
//	dprintf("-WStringType::getStringData\n");
//}
//
//
