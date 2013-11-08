// this must be included before stdafx.h
#include "parser/CParser.tab.h"

#include "stdafx.h"
#include "Compiler.h"
#include <strstream>
#include "parser/CLexer.h"
#include "Runtime.h"
#include "PBException.h"

//typedef int *FN1(), FN2();
//int *fn();
//FN1 v1;
//FN2 v2;

Compiler::Compiler(const string &nameSpace, bool enableTCharSuffix)
	: m_nameSpace(nameSpace)
	, m_enableTCharSuffix(m_enableTCharSuffix)
{
	//v1();
}

Compiler::~Compiler()
{

}


string  Compiler::compileAsC(const char* source)
{
	dprintf("compileAsC\n%s\n", source);
	yy::CParser parser(*this);
	istrstream is(source);
	CLexer lexer(&is);
	int result = parser.parse();
	if (result != 0) 
		throw RuntimeException(/*"Syntax error: " + */ m_errorMessage);
	if (m_names.size() == 0) 
		return "";
	else
		return m_names[m_names.size() - 1];
}
string  Compiler::compileAsCTypedef(const string &source)
{
	string typedefSource = "typedef " + source;
	dprintf("compileAsCTypedef: %s\n", typedefSource.c_str());
	yy::CParser parser(*this);
	istrstream is(typedefSource.c_str());
	CLexer lexer(&is);
	int result = parser.parse();
	if (result != 0) 
		throw RuntimeException(/*"Syntax error: " + */ m_errorMessage);
	if (m_typeDefs.size() == 0) 
		return "";
	else
		return m_typeDefs[m_typeDefs.size() - 1];
}
Type Compiler::findType(const string& typeName, bool throwNotFound)
{
	Type type = m_tags[typeName];
	if (type.get() == NULL)
		type = CoreInstance.findType(m_nameSpace, typeName, throwNotFound);
	return type;
}

Type Compiler::defineType(const string& typeName, Type &typeRef)
{
	if (typeName != "") {
		Type type = CoreInstance.defineType(m_nameSpace, typeName, typeRef);
		m_typeDefs.push_back(typeName);
		return type;
	}
	else {
		return typeRef;
	}
}
Type Compiler::defineTag(const string& typeName, Type &typeRef)
{
	if (typeName != "") {
		m_tags[typeName] = typeRef;
	}
	return typeRef;
}

Type Compiler::defineName(string name, Type &typeRef)
{
	if (typeRef->isVoid())
		throw RuntimeException("Illegal use of 'void' type for declaring '" + name + "'");
	if (m_enableTCharSuffix && typeRef->hasTChar()) {
		if (CoreInstance.isUnicode()) {
			name += "W";
		}
		else {
			name += "A";
		}
	}
	Type type = CoreInstance.defineName(m_nameSpace, name, typeRef);
	m_names.push_back(name);
	return type;
}
