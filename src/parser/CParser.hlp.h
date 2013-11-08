//#pragma message("inlude CParser.hlp.h")
#pragma once
#pragma warning (disable:4065)
#include "../stdafx.h"
#include "../types/BaseType.h"
#include "../Compiler.h"
#include "../Core.h"
#include "ASTNode.h"

//#include <iostream>
//#include <string>

using namespace std;

struct YYSTYPE {
	string text;
	Type type;
	ASTNode node;
	vector<NamedType> items;
	NamedType namedType;
	int ival;
};
#define YYSTYPE_IS_DECLARED 1

//#define yylex CLexerlex
int yylex(YYSTYPE* yylval);

//class Compiler;
