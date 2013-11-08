#pragma once

#include "BaseType.h"


class TypeFactory {
private:
	static TypeFactory * m_instance;
	TypeFactory() {}
	TypeFactory(TypeFactory&) {}
	TypeFactory& operator=(TypeFactory&) {}
public:
	static TypeFactory& getInstance();
	Type createType(const string &typeName);
	Type createMethodType(const char* methodDecl);
};

