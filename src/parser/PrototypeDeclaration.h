#pragma once
#include "Declaration.h"
#include "../types/BaseType.h"


class PrototypeDeclaration: public IDeclaration
{
public:
	PrototypeDeclaration();
	~PrototypeDeclaration(void);

	string m_name;
	size_t m_align;
	bool m_isStdCall;
	vector<Type> m_paramTypes;
	Type m_returnType;
};

inline PrototypeDeclaration* proto(Declaration &decl) 
{
	return (PrototypeDeclaration*) decl.get();
}