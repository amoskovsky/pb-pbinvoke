#include "../stdafx.h"

#include "MethodType.h"
#include "../Runtime.h"
#include "../Core.h"
#include "../PBException.h"

IndexedMethodType::IndexedMethodType(Type returnType, vector<Type> &paramTypes, bool isStdCall, DWORD methodAddr)
	: m_returnType(returnType)
	, m_library(NULL)
	, m_methodName("")
	, m_paramTypes(paramTypes)
	, m_isStdCall(isStdCall)
	, m_methodAddr(methodAddr)
	, RawType("")
{
	//dprintf("IndexedMethodType::IndexedMethodType\n");
	//dprintf(" m_returnType.ptr=%p\n", m_returnType.get());
	//dprintf(" getReturnType().get()=%p\n", getReturnType().get());
	//dprintf(" getTypeSignature='%s'\n", getTypeSignature().c_str());
	//dprintf(" getTypeSignature ptr='%s'\n", getTypeSignature("*").c_str());
}

MethodType::MethodType(Type returnType, vector<NamedType> &params, bool isStdCall)
	: m_params(params)
	, m_indexedMethodType((IndexedMethodType*) CoreInstance.registerType(Type(
		new IndexedMethodType(returnType, NamedType::getTypes(params), isStdCall)
		)).get())
	, RawType("")
{
	dprintf("MethodType::MethodType\n");
	dprintf(" params.size()=%i\n", params.size());
	//dprintf(" getReturnType().get()=%p\n", getReturnType().get());
	//dprintf(" getTypeSignature='%s'\n", getTypeSignature().c_str());
	//dprintf(" getTypeSignature ptr='%s'\n", getTypeSignature("*").c_str());
	init();
}

void MethodType::init()
{
	for (size_t i = 0; i < m_params.size(); i ++) {
		m_paramNameMap[m_params[i].name] = i + 1;
	}
}

bool IndexedMethodType::hasTChar()
{
	if (m_returnType->hasTChar())
		return true;
	for (size_t i = 0; i < m_paramTypes.size(); i ++) {
		if (m_paramTypes[i]->hasTChar())
			return true;
	}
	return false;
}

int MethodType::getItemIndex(const string& itemName)
{
	int index = m_paramNameMap[itemName];
	if (index == 0)
		throw RuntimeException("No function parameter with name '" + itemName + "'");
	return index - 1; // 1-based are stored, but 0-based are needed.
}


bool MethodType::isCompatible(BaseType* type) 
{ 
	return type != NULL && 
		type->getPBTypeClass() == getPBTypeClass() &&
		m_indexedMethodType == ((MethodType*)type)->m_indexedMethodType
		;
}


string IndexedMethodType::getTypeSignature(const string& pointerSig)
{
	return buildTypeSignature(pointerSig, m_methodName, m_returnType, m_paramTypes, m_isStdCall);
}


IndexedMethodType::~IndexedMethodType()
{
	//dprintf("IndexedMethodType::~IndexedMethodType\n");

}

string IndexedMethodType::buildTypeSignature(const string &pointerName, const string &methodName, Type returnType, vector<Type> &paramTypes, bool isStdCall)
{
	string decl = isStdCall ? "__stdcall" : "__cdecl"; 
	string sig;
	sig = returnType->getTypeSignature() + string(" ");
	if (pointerName != "") {
		sig += "(" + decl + " " + pointerName + ")";
	}
	else {
		sig += decl ;//+ " " + methodName;
	}
	sig += "_f(";
	string sep;
	for (size_t i = 0; i < paramTypes.size(); i ++) {
		sig += sep + paramTypes[i]->getTypeSignature();
		sep = ", ";
	}
	sig += ")";

	return sig;
}


//DWORD IndexedMethodType::getMethodAddr()
//{
//	if (m_methodAddr == 0) {
//		m_methodAddr = m_library->getMethodAddr(m_methodName);
//	}
//	return m_methodAddr;
//}

//#pragma pack (push, 4)

Method * PBCALL me_create(MethodType *methodType) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		return new Method(methodType);
	}
	catch (...) {
		return NULL;
	}
}


BOOL PBCALL mt_getMethodTypeData1(MethodType* methodType, BaseType* &returnType, int &returnTypeClass, int &paramCount)
{
	dprintf("mt_getMethodTypeData1 methodType=%p\n", methodType);
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		dprintf(" 1 \n");
		returnType = methodType->getReturnType().get();
		dprintf(" 2\n");
		returnTypeClass = returnType->getPBTypeClass();
		dprintf(" 3\n");
		paramCount = methodType->getParams().size();
		dprintf("returnType=%p, returnTypeClass=%i, paramCount=%i\n", returnType, returnTypeClass, paramCount);
		return true;
	}
	catch (exception const & e) {
		dprintf("catch (exception const & e): %s\n", e.what());
	}
	catch (...) {
		dprintf("catch \n");
	}
	return false;
}

BOOL PBCALL mt_getMethodTypeData2(MethodType* methodType, BaseType* paramTypes[], int paramTypeClasses[], size_t paramCount)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		vector<NamedType> &params = methodType->getParams();
		if (paramCount > params.size())
			paramCount = params.size();
		for (size_t i = 0; i < paramCount; i ++) {
			paramTypes[i] = params[i].type.get();
			paramTypeClasses[i] = paramTypes[i]->getPBTypeClass();
		}
		return true;
	}
	catch (...) {
		return false;
	}

	return true;
}






void PBCALL me_destroy(Method *method) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	if(method != NULL) {
		delete method;
	}
}


BOOL PBCALL me_getContextData(Method *method, BYTE* &returnValueAddr, BYTE* paramAddr[], int paramCount) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		for (int i = 0; i < paramCount; i ++) {
			paramAddr[i] = method->getParamAddr(i);
		}
		returnValueAddr = method->getReturnValueAddr();
		return true;
	}
	catch (...) {
		return false;
	}
}


string MethodType::getTypeSignature(const string& pointerSig)
{
	return buildTypeSignature(pointerSig);
}



string MethodType::buildTypeSignature(const string &pointerName)
{
	string decl = isStdCall() ? "__stdcall" : "__cdecl"; 
	string sig;
	sig = getReturnType()->getTypeSignature() + string(" ");
	if (pointerName != "") {
		sig += "(" + decl + " " + pointerName + ")";
	}
	else {
		sig += decl ;//+ " " + m_methodName;
	}
	sig += "(";
	string sep;
	for (size_t i = 0; i < m_params.size(); i ++) {
		sig += sep + m_params[i].type->getTypeSignature();
		if (m_params[i].name != "")
			sig += " " + m_params[i].name;
		sep = ", ";
	}
	sig += ")";

	return sig;
}
