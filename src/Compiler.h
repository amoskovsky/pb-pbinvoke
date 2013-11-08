#pragma once

#include "parser/Declaration.h"
//#include "types/StructType.h"
#include <hash_map>
using namespace stdext;
class Compiler  
{
private:
	Declarations m_declarations;
	string m_errorMessage;
	const string m_nameSpace;
	vector<string> m_typeDefs;
	vector<string> m_names;
	hash_map<string, Type> m_tags;
	bool m_enableTCharSuffix;

public:
	Compiler(const string &nameSpace = "", bool enableTCharSuffix = true);
	virtual ~Compiler();
	/** @return name of the last name def or "" */
	string compileAsC(const char* source);
	string compileAsCTypedef(const string &source);
	inline Declarations& getDeclarations() { return m_declarations; }
	inline void addDeclaration(Declaration& decl) { m_declarations.push_back(decl); }
	inline void setErrorMessage(const string& message) { m_errorMessage = message; } 
	Type findType(const string& typeName, bool throwNotFound = true);
	Type defineType(const string& typeName, Type &typeRef);
	Type defineName(string name, Type &typeRef);
	Type defineTag(const string& typeName, Type &typeRef);
};

