#pragma once
#pragma warning (disable:4065)

#include "CParser.tab.h"
#include <iostream>
#include <stack>
using namespace std;
//namespace yy {
	//class CLexer: public yyFlexLexer {
	//	public:
	//	int yylex(YYSTYPE* yylval);
	//};

//} 

class CLexer {
	static istream* m_inputStream;
	istream* m_prevInputStream;
	istream* m_currInputStream;
public:
	inline CLexer(istream* is /*caller must delete is*/);
	void pushInput();
	void popInput();
	static inline istream* in() {return m_inputStream; }
	inline ~CLexer();
};

inline CLexer::CLexer(istream* is)
	: m_currInputStream(is)
	, m_prevInputStream(NULL)
{
	if (m_currInputStream == NULL)
		throw "m_currInputStream == NULL";
	pushInput();
	m_inputStream = m_currInputStream;
}
inline CLexer::~CLexer()
{
	popInput();
}
