%{
#include "CLexer.h"
#include <io.h>

//#define YY_DECL int CLexer::yylex(YYSTYPE* yylval)
//#define YY_DECL int yyFlexLexer::yylex(YYSTYPE* yylval)

#define YY_DECL int yylex(YYSTYPE* yylval)
#define YY_INPUT(buf,result,max_size) \
   { 	\
   	CLexer::in()->read(buf, max_size);\
   	result = CLexer::in()->bad() ? YY_NULL : CLexer::in()->gcount();\
   }


	stack<YY_BUFFER_STATE> m_buffers;


%}

%option 8bit

/*%option c++ */
/*%option yyclass="CLexer"*/
%option outfile="CLexer.cpp"
%option nounistd 
/*%option prefix="CLexer" */

id0  [a-zA-Z_]
id   {id0}({id0}|[0-9@])*
int  [0-9]+
num1 {int}("."{int})?([eE][+-]?{int})?
num2 "."{int}([eE][+-]?{int})?
num  {num1}|{num2}
sp   [ \t]*
sp1  [ \t]+


%x COMM

%%

[ \t\r\n]+  /*skip spaces*/
"//"[^\n]*  /*// comments*/
"/*"          { BEGIN(COMM); } /* / * * / comments*/
<COMM>[^\*]+  { }      
<COMM>"*"+    { }      
<COMM>"*"+"/" { BEGIN(INITIAL); }
NEAR|FAR|near|far  {}     
typedef     { return T_TYPEDEF; } 
__cdecl|CDECL                                     { return ATTR_CDECL; } 
__stdcall|CALLBACK|WINAPI|STDCALL|PASCAL|pascal   { return ATTR_STDCALL; } 
struct      { return T_STRUCT; } 
union       { return T_UNION; } 
enum        { return T_ENUM; } 
const       /* ignore */  
unsigned    { return T_UNSIGNED; }
signed      { return T_SIGNED; }
char        { return T_CHAR;}
wchar_t     { return T_WCHAR_T;}
int         { return T_INT;}
__int8      { return T_INT8;}
__int16     { return T_INT16;}
__int32     { return T_INT32;}
__int64     { return T_INT64;}
short       { return T_SHORT;}
long        { return T_LONG;}
longlong    { return T_LONGLONG;}
double      { return T_DOUBLE;}
float       { return T_FLOAT;}
void        { return T_VOID;}
__declspec  { return ATTR_DECLSPEC; }
";"         { return SEMICOLON; } 
"::"        { return DBLCOLON; } 
"="         { return ASSIGN; } 
","         { return COMMA; } 
"("         { return LPAREN; } 
")"         { return RPAREN; } 
"{"         { return LCURL; } 
"}"         { return RCURL; } 
"["         { return LBRACKET; } 
"]"         { return RBRACKET; } 
"*"         { return STAR; } 
"&"         { return ADDR; } 
{id}        { yylval->text = yytext; return ID; } 
{num}       { yylval->text = yytext; return NUM; } 
"#"{sp}pragma{sp1}pack { return T_PRAGMA_PACK; }
"#"{sp}pragma[^\n]*    /* ignore unknown pragmas */
.           /* ignore */
%%


int yywrap() 
{
	return 1; // terminate on eof
}

istream * CLexer::m_inputStream;

void CLexer::pushInput()
{
	m_prevInputStream = m_inputStream;
	m_buffers.push(YY_CURRENT_BUFFER);
   yy_switch_to_buffer(yy_create_buffer(NULL, YY_BUF_SIZE));
}
void CLexer::popInput()
{
   yy_delete_buffer(YY_CURRENT_BUFFER);
	m_inputStream = m_prevInputStream;
   yy_switch_to_buffer(m_buffers.top());
   m_buffers.pop();
}

