#include "../stdafx.h"
#include "ASTNode.h"

#include "../types/PointerType.h"
#include "../types/RefType.h"
#include "../types/MethodType.h"
#include "../types/StructType.h"
#include "../types/ArrayType.h"
#include "../Core.h"

//typedef int iii;
//typedef int iii;

int __cdecl fn1();
int  *__cdecl fn2();
int  ( __cdecl *  fn3)();
//int  (  * __cdecl fn4)(); //warning C4229: anachronism used : modifiers on data are ignored
//typedef __declspec(align(1)) struct  S1 {}; //warning C4091: 'typedef ' : ignored on left of 'S1' when no variable is declared
//__declspec(align(1)) struct  S2 {};


void ASTDeclaratorId::convert(Type typeRef, NamedType& namedType)
{	
	namedType.name = id;
	namedType.type = typeRef;
}

void ASTPtrTypeOp::convert(Type typeRef, NamedType& namedType)
{
	arg->getDeclarator()->convert(Type(new PointerType(typeRef)), namedType);
}

void ASTRefTypeOp::convert(Type typeRef, NamedType& namedType)
{
	arg->getDeclarator()->convert(Type(new RefType(typeRef)), namedType);
}

void ASTArrayTypeOp::convert(Type typeRef, NamedType& namedType)
{
	if (/*m_bound == 0 ||*/ m_isParam) { 
		typeRef = Type(new PointerType(typeRef));
	}
	else {
		typeRef = Type(new ArrayType(typeRef, m_bound));
	}
	m_arg->getDeclarator()->convert(typeRef, namedType);
}


void ASTMethodTypeOp::convert(Type typeRef, NamedType& namedType)
{
	Type methodType = Type(new MethodType(typeRef, params, isStdCall));
	id->getDeclarator()->convert(methodType, namedType);
}

void ASTNodeList::typeDef(Type typeRef, Compiler& compiler)
{
	for (size_t i = 0; i < list.size(); i ++) {
		NamedType namedType;
		list[i]->getDeclarator()->convert(typeRef, namedType);
		compiler.defineType(namedType.name, namedType.type);
	}
}




void ASTNodeList::nameDef(Type typeRef, Compiler& compiler)
{
	for (size_t i = 0; i < list.size(); i ++) {
		NamedType namedType;
		list[i]->getDeclarator()->convert(typeRef, namedType);
		compiler.defineName(namedType.name, namedType.type);
	}
}
void ASTNodeList::nameDef(Type typeRef, vector<NamedType> &names, Compiler& compiler)
{
	for (size_t i = 0; i < list.size(); i ++) {
		NamedType namedType;
		list[i]->getDeclarator()->convert(typeRef, namedType);
		names.push_back(namedType);
	}
}
//void ASTDeclaratorId::setStdCall(bool _isStdCall) 
//{ 
//	throw RuntimeException("Cannot apply " + string(_isStdCall ? "__stdcall":"__cdecl") +
//		" to identifier '" + id + "'");
//}


ASTStructType::ASTStructType(const string& _name, Compiler &_compiler)
	: name(_name) 
	, compiler(_compiler)
{ 
	forwardDecl();
}

void ASTStructType::completeStruct(int structTypeId, vector<NamedType> &fields)
{
	bool isUnion;
	if (structTypeId == TC_STRUCT_TYPE)
		isUnion = false;
	else if (structTypeId == TC_UNION_TYPE)
		isUnion = true;
	else
		throw RuntimeException("Internal error: Uknown struct type ID used: " + string(CUtil::toString(structTypeId)));
	
	((StructType*)structType.get())->complete(fields, CoreInstance.getPack(), 0, isUnion); // TODO add align (declspec)
	
	dprintf("defineStruct, type=%p, sig=%s\n", structType.get(), structType->getTypeSignature("*").c_str());	
	structType = compiler.defineType(name, structType);
}

Type ASTStructType::getTypeRef()
{
	return structType;
}

void ASTStructType::forwardDecl()
{
	structType = Type(new StructType(name));
	compiler.defineTag(name, structType);
}


ASTInternalType::ASTInternalType(Compiler &_compiler)
	: compiler(_compiler)
	, attrs (0)
{

}
void ASTInternalType::setTypeAttr(int attr) 
{ 
	attrs = attrs | attr; 
}


Type ASTInternalType::getTypeRef()
{
	return compiler.findType(getInternalTypeName(attrs));	
}

string ASTInternalType::getInternalTypeName(int attrs)
{
	// check check combinations of signed/unsigned 
	if ((attrs & ATTR_SIGNED) && (attrs & ATTR_UNSIGNED)) 
		throw RuntimeException("Signed/unsigned keywords mutually exclusive");
	// clear signed/unsigned - it's just ignored
	attrs = attrs & (~ (ATTR_SIGNED | ATTR_UNSIGNED));

	//check combinations of internal types
	int currAttr = 1;
	int usedAttrs = 0;
	for (int i = 0; i < ATTR_COUNT; i ++, currAttr = currAttr << 1) {
		if (attrs & currAttr) 
			usedAttrs ++;
	}
	if (usedAttrs > 1)
		throw RuntimeException("Illegal use of internal types: more than one type specified");

	// default type: int
	if (usedAttrs == 0)
		attrs = ATTR_INT;

	// map type implementations
#define MAP_TYPE(s, t) if (attrs & s) { attrs = (attrs | t) & ~s;}
	MAP_TYPE(ATTR_INT8, ATTR_CHAR);
	MAP_TYPE(ATTR_INT16, ATTR_SHORT);
	MAP_TYPE(ATTR_INT32, ATTR_INT);
	MAP_TYPE(ATTR_LONG, ATTR_INT);
	MAP_TYPE(ATTR_LONGLONG, ATTR_INT64);

	// map attrs to type name
#define MAP_TYPE_NAME(s, t) case s: typeName = t; break;
	string typeName;
	switch (attrs) {
	MAP_TYPE_NAME(ATTR_VOID, "void")
	MAP_TYPE_NAME(ATTR_CHAR, "char")
	MAP_TYPE_NAME(ATTR_WCHAR_T, "wchar_t")
	MAP_TYPE_NAME(ATTR_INT8, "__int8")
	MAP_TYPE_NAME(ATTR_INT16, "__int16")
	MAP_TYPE_NAME(ATTR_SHORT, "short")
	MAP_TYPE_NAME(ATTR_INT, "int")
	MAP_TYPE_NAME(ATTR_LONG, "long")
	MAP_TYPE_NAME(ATTR_INT32, "__int32")
	MAP_TYPE_NAME(ATTR_INT64, "__int64")
	MAP_TYPE_NAME(ATTR_LONGLONG, "longlong")
	MAP_TYPE_NAME(ATTR_FLOAT, "float")
	MAP_TYPE_NAME(ATTR_DOUBLE, "double")
	default:
		throw RuntimeException("Internal error: unknown type attr " + string(CUtil::toString(attrs)));
	}
	return typeName;
}

ASTUserType::ASTUserType(const string& _name, Compiler &_compiler)
	: name(_name)
	, compiler(_compiler)
{

}

Type ASTUserType::getTypeRef()
{
	return compiler.findType(name);
}

