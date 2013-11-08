#include "../stdafx.h"

#include "../Runtime.h"
#include "../PBException.h"
#include "PointerType.h"
#include "MethodType.h"
#include "../Core.h"


BaseType::BaseType()
{
}

BaseType::~BaseType()
{

}
bool BaseType::isPointerToMethod()
{
	if (getPBTypeClass() == TC_PTR_TYPE) {
		if (((PointerType*)this)->getDataType()->getPBTypeClass() == TC_METHOD_TYPE )
			return true;
	}
	return false;
}

BOOL PBCALL bt_setData(BaseType *dataType, ExecutionContext *context, BYTE * cBuf, BYTE * pbBuf)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		dataType->setData(context, cBuf, pbBuf);
		return true;
	}
	catch (...) {
		return false;
	}
}
//BOOL PBCALL bt_setPtr(BaseType *dataType, ExecutionContext *context, BYTE * cBuf, BYTE * pbBuf)
//{
//	PBExceptionStorageInstance.clearLastExceptionMessage();
//	try {
//		dataType->setData(context, cBuf, pbBuf);
//		return true;
//	}
//	catch (...) {
//		return false;
//	}
//}

BOOL PBCALL bt_getData(BaseType *dataType, ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		dataType->getData(context, cBuf, pbBuf);
		return true;
	}
	catch (...) {
		return false;
	}
}

BOOL PBCALL bt_setArrayData(BaseType *dataType, ExecutionContext *context, BYTE * &cBuf, BYTE *pbBuf, int arrayLen, DataCharset charset)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		dataType->setArrayData(context, cBuf, pbBuf, arrayLen, charset);
		return true;
	}
	catch (...) {
		return false;
	}
}

BOOL PBCALL bt_getArrayData(BaseType *dataType, ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &arrayLen, DataCharset charset) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		dataType->getArrayData(context, cBuf, pbBuf, arrayLen, charset);
		return true;
	}
	catch (...) {
		return false;
	}
}

int PBCALL bt_getPBTypeClass(BaseType* dataType) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	return dataType->getPBTypeClass();
}

int PBCALL bt_getSizeOf(BaseType* dataType) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	if (dataType == NULL)
		return 0;
	return dataType->getSizeOf();
}

BOOL PBCALL g_isPBUnicode(BYTE * unicodeMarker) 
{
	dprintf("+g_isPBUnicode\n");
	dprintf(" unicodeMarker='%s'\n", unicodeMarker);
	bool isUnicode = IsUnicode(unicodeMarker);
	CoreInstance.setUnicode(isUnicode);
	dprintf("-g_isPBUnicode isUnicode=%i\n", isUnicode);
	return isUnicode;
	//return IsUnicode(unicodeMarker);
}

BOOL PBCALL bt_allocateBuffer(BaseType* dataType, ExecutionContext *context, BYTE * &buf, int itemCount)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		dataType->allocateBuffer(context, buf, itemCount);
		return true;
	}
	catch (...) {
		return false;
	}
}

BOOL PBCALL bt_setZeroData(BaseType* dataType, ExecutionContext *context, BYTE * buf)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		dataType->setZeroData(context, buf);
		return true;
	}
	catch (...) {
		return false;
	}
}


vector<NamedType> NamedType::getNamedTypes(vector<Type> &v)
{
	vector<NamedType> result(v.size());
	for (size_t i = 0; i < v.size(); i ++) {
		result[i].type = v[i];
	}
	return result;
}

vector<Type> NamedType::getTypes(vector<NamedType> &v)
{
	vector<Type> result(v.size());
	for (size_t i = 0; i < v.size(); i ++) {
		result[i] = v[i].type;
	}
	return result;
}

vector<string> NamedType::getNames(vector<NamedType> &v)
{
	vector<string> result(v.size());
	for (size_t i = 0; i < v.size(); i ++) {
		result[i] = v[i].name;
	}
	return result;
}
vector<string> NamedType::getNames(vector<Type> &v)
{
	vector<string> result(v.size());
	return result;
}


//void BaseType::setPointerToArrayData(ExecutionContext *context, BYTE * cPtrBuf, BYTE *pbBuf, int arrayLen, DataCharset charset)
//{
//	BYTE * cBuf = NULL;
//	if (pbBuf != NULL) {
//		setArrayData(context, cBuf, pbBuf, arrayLen, charset);
//	}
//	setData(context, cPtrBuf, (BYTE*)&cBuf);
//}

//BOOL PBCALL bt_setPointerToArrayData(BaseType* dataType, ExecutionContext *context, BYTE * cPtrBuf, BYTE *pbBuf, int arrayLen, DataCharset charset)
//{
//	PBExceptionStorageInstance.clearLastExceptionMessage();
//	try {
//		dataType->setPointerToArrayData(context, cPtrBuf, pbBuf, arrayLen, charset);
//		return true;
//	}
//	catch (...) {
//		return false;
//	}
//} 

BOOL PBCALL bt_getPtr(BaseType* dataType, ExecutionContext *context, BYTE* cBuf, BYTE* &pbBuf)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		dataType->getPtr(context, cBuf, pbBuf);
		return true;
	}
	catch (...) {
		return false;
	}
}
BOOL PBCALL bt_setPtr(BaseType* dataType, ExecutionContext *context, BYTE* cBuf, BYTE* pbBuf)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		dataType->setPtr(context, cBuf, pbBuf);
		return true;
	}
	catch (...) {
		return false;
	}
}

BOOL PBCALL bt_isCompatible(BaseType *dataType1, BaseType *dataType2)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		return dataType1->isCompatible(dataType2);;
	}
	catch (...) {
		return false;
	}
}

BOOL PBCALL bt_isSame(BaseType *dataType1, BaseType *dataType2)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		return dataType1->isSame(dataType2);;
	}
	catch (...) {
		return false;
	}
}

int PBCALL bt_getItemIndex(BaseType *dataType, const char* itemName, DataCharset charset)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		ParamStorage ps;
		if (charset == DC_UNICODE)
			itemName = ps.convertWtoA((wchar_t*)itemName);
		return dataType->getItemIndex(itemName) + 1; // PB index is 1-based
	}
	catch (...) {
		return 0;
	}
}

inline void BaseType::setInt64(ExecutionContext *context, __int64* cBuf, BYTE* str, int strLen, DataCharset charset)
{
	if (charset == DC_UNICODE) {
		*cBuf = _wtoi64((wchar_t*)str);
	}
	else {
		*cBuf = _atoi64((char*)str);
	}
}
BOOL PBCALL bt_setInt64(BaseType* baseType, ExecutionContext *context, __int64* cBuf, BYTE* str, int strLen, DataCharset charset)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		baseType->setInt64(context, cBuf, str, strLen, charset);
		return true;
	}
	catch (...) {
		return false;
	}
} 

inline void BaseType::getInt64(ExecutionContext *context, __int64* cBuf, string* &str)
{
	vector<char> buf(100);
	_i64toa(*cBuf, &buf[0], 10);
	str = new string(&buf[0]);
}

BOOL PBCALL bt_getInt64(BaseType* baseType, ExecutionContext *context, __int64* cBuf, string* &str)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		baseType->getInt64(context, cBuf, str);
		return true;
	}
	catch (...) {
		return false;
	}
}