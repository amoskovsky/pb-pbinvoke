#include "../stdafx.h"

#include "TypeFactory.h"
#include "../Runtime.h"
#include "../Core.h"
#include "DataTypes.h"
#include "../PBException.h"
#include "../Compiler.h"



TypeFactory * TypeFactory::m_instance = NULL;


Type TypeFactory::createType(const string &typeName)
{
	return CoreInstance.findType("", typeName);
}




TypeFactory& TypeFactory::getInstance()
{
	if (m_instance == NULL)
		m_instance = new TypeFactory();
	return *m_instance;
}

BaseType* PBCALL bt_create(char *typeName, DataCharset charset) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	ParamStorage ps;
	if (charset == DC_UNICODE)
		typeName = ps.convertWtoA((wchar_t*)typeName);
	try {
		Type typeRef = CoreInstance.findType("", typeName);
		return typeRef.get();
	}
	catch (...) {
		return NULL;
	}
}


Type TypeFactory::createMethodType(const char* methodDecl) 
{
	// try 1st if method ptr name was passed 
	string methodPtrName = methodDecl;
	Type methodPtrType = CoreInstance.findType("", methodPtrName, false);
	if (methodPtrType.get() == NULL) {
		// no method with methodDecl name
		// compile declaration
		Compiler compiler("");
		methodPtrName = compiler.compileAsCTypedef(methodDecl);
		if (methodPtrName == "") 
			throw RuntimeException("Declaration does not contain function declarations: '" + string(methodDecl) + "'");
		methodPtrType = CoreInstance.findType("", methodPtrName);
	}
	dprintf("TypeFactory::createMethodType: class=%i, decl='%s', sig='%s'\n", methodPtrType->getPBTypeClass(), methodPtrName.c_str(), methodPtrType->getTypeSignature().c_str());
	if (methodPtrType->getPBTypeClass() == TC_METHOD_TYPE) {
		return methodPtrType;
	}
	else if (methodPtrType->isPointerToMethod()) {
		return ((PointerType*)methodPtrType.get())->getDataType();
	}
	else {
		throw RuntimeException("Name '" + methodPtrName + "' is not a 'function' declaration");
	}
	
}

MethodType* PBCALL tf_createMethodType(const char *methodDecl, DataCharset charset) 
{
	dprintf("tf_createMethodType charset=%i\n", charset);
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		ParamStorage ps;
		if (charset == DC_UNICODE) {
			methodDecl = ps.convertWtoA((wchar_t*)methodDecl);
		}
		return (MethodType*)TypeFactory::getInstance().createMethodType(methodDecl).get();
	}
	catch(...) {
		return NULL;
	}
}





