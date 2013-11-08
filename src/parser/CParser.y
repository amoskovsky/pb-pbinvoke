%{

#include "CParser.hlp.h"

%} 

//%union {
//	char* text;
//}

%skeleton "lalr1.cc"
%define "parser_class_name" "CParser"
%defines
%pure-parser
%parse-param {Compiler& compiler}
//%lex-param {CPbParserDriver *driver}
%debug 
%error-verbose 



%token ID NUM

%token T_TYPEDEF ATTR_CDECL ATTR_STDCALL T_STRUCT T_UNION T_ENUM ATTR_CONST ATTR_DECLSPEC T_PRAGMA_PACK 

%token T_UNSIGNED T_SIGNED T_INT8 T_INT16 T_INT32 T_INT64 
%token T_DOUBLE T_FLOAT  T_CHAR T_WCHAR_T T_INT T_LONG T_LONGLONG T_SHORT T_VOID

%token SEMICOLON DBLCOLON ASSIGN COMMA LPAREN RPAREN LCURL RCURL LBRACKET RBRACKET STAR ADDR 

%token PARAM_SEQ ATTR_PRE ATTR_POST TEST_RULE LOW  HIGH

%type <text> ID NUM 
%type <node> declarator declarator_seq maybe_declarator_seq 
%type <node> param_declarator /*param_declarator_arg*/  declarator_arg 
%type <node> attributed_type internal_type user_type  struct_type  struct_tag

%type <items> maybe_param_seq param_seq  var_decl var_decl_seq
%type <namedType> param 
%type <type> type_ref 
%type <ival> call_modifier T_STRUCT T_UNION  struct attr array_bound

// upper decl - lower priority
%left SEMICOLON
%left COMMA 
%left LPAREN
%left STAR
%nonassoc ADDR 
%nonassoc PARAM_SEQ
//%right ATTR_POST
//%left ATTR_PRE

//%left LOW
//%left HIGH
//%left T_SIGNED T_UNSIGNED T_INT8 T_INT16 T_INT32 T_INT64 T_DOUBLE T_FLOAT




%%

prog:
	decl_seq           
;

decl_seq:
	sep
|	sep decl						   
|	decl
|	decl_seq sep decl
|	decl_seq sep
;

sep:
	SEMICOLON
|	pragma
;

decl:
	typedef
|	namedef
|	namedef2
;

pragma:
	pragma_pack
;
pragma_pack:
	T_PRAGMA_PACK LPAREN NUM RPAREN            { CoreInstance.setPack(atoi($3.c_str()));}
|  T_PRAGMA_PACK LPAREN ID RPAREN             { 
                                                if ($3 != "pop") 
                                                	throw RuntimeException("#pragma pack: unexpected " + $3);
                                                CoreInstance.popPack();
                                              }
|  T_PRAGMA_PACK LPAREN ID COMMA NUM RPAREN   { 
                                                if ($3 != "push") 
                                                	throw RuntimeException("#pragma pack: unexpected " + $3);
                                                CoreInstance.pushPack(atoi($5.c_str()));
                                              }
;

namedef:
	type_ref maybe_declarator_seq              { $2->getNodeList()->nameDef($1, compiler); }
;


typedef:
	T_TYPEDEF type_ref maybe_declarator_seq    { $3->getNodeList()->typeDef($2, compiler); }
;


namedef2:
	TEST_RULE type_ref maybe_declarator_seq  
;



type_ref:
	attributed_type               { $$ = $1->getTypeRef(); } 
;

attributed_type:
   internal_type                
|	user_type
|	declspec attributed_type      { $$ = $2; /*ignore declspec yet*/ }
;

internal_type:
	attr                          { $$ = ASTNode(new ASTInternalType(compiler)); $$->setTypeAttr($1); }
|	internal_type attr            { $$ = $1; $$->setTypeAttr($2); }
|	internal_type declspec        { $$ = $1; /*ignore declspec yet*/ }
;

attr:
	T_SIGNED                      { $$ = ATTR_SIGNED; }
|	T_UNSIGNED                    { $$ = ATTR_UNSIGNED; }
|	T_CHAR                        { $$ = ATTR_CHAR; }
|	T_INT8                        { $$ = ATTR_INT8; }
|	T_WCHAR_T                     { $$ = ATTR_WCHAR_T; }
|	T_SHORT                       { $$ = ATTR_SHORT; }
|	T_INT16                       { $$ = ATTR_INT16; }
|	T_INT32                       { $$ = ATTR_INT; }
|	T_INT                         { $$ = ATTR_INT; }
|	T_INT64                       { $$ = ATTR_INT64; }
|	T_LONG                        { $$ = ATTR_INT; }
|	T_LONGLONG                    { $$ = ATTR_LONGLONG; }
|	T_DOUBLE                      { $$ = ATTR_DOUBLE; }
|	T_FLOAT                       { $$ = ATTR_FLOAT; }
|	T_VOID                        { $$ = ATTR_VOID; }
;

user_type:
	ID                            { $$ = ASTNode(new ASTUserType($1, compiler)); }
|	struct_type                   { $$ = $1; }
|  user_type  declspec           { $$ = $1; /*ignore declspec yet*/}
;

struct_type:
	struct ID                                                { $$ = ASTNode(new ASTUserType($2, compiler)); }
|	struct struct_tag  LCURL var_decl_seq RCURL              { $$ = $2; $$->getStructType()->completeStruct($1, $4); }
;

declspec:
	ATTR_DECLSPEC LPAREN ID RPAREN       
|	ATTR_DECLSPEC LPAREN ID LPAREN NUM RPAREN RPAREN       
;


struct:
	T_STRUCT         { $$ = TC_STRUCT_TYPE; }
|	T_UNION          { $$ = TC_UNION_TYPE; }
;

struct_tag:                      // add forward decl here
	/*empty*/                     { $$ = ASTNode(new ASTStructType("", compiler)); }
|	ID                            { $$ = ASTNode(new ASTStructType($1, compiler)); } 
|	declspec                      { $$ = ASTNode(new ASTStructType("", compiler)); }
|	declspec ID                   { $$ = ASTNode(new ASTStructType($2, compiler)); }
;


var_decl_seq:
	/*empty*/                        { $$.clear(); }
|	var_decl_seq var_decl SEMICOLON  { $$ = $1; $$.insert($$.end(), $2.begin(), $2.end()); }
;

var_decl:
	type_ref declarator_seq                    { $2->getNodeList()->nameDef($1, $$, compiler); }
;


maybe_declarator_seq:
                                              { $$ = ASTNode(new ASTNodeList()); }
|	declarator_seq	                            
;


declarator_seq:
	declarator                                 { $$ = ASTNode(new ASTNodeList($1)); }
|	declarator_seq COMMA declarator            { $$ = $1; $$->getNodeList()->list.push_back($3); }
;


declarator:
	declarator_arg                                      { $$ = $1; }
|	LPAREN declarator RPAREN LPAREN maybe_param_seq RPAREN    
	                                                    { $$ = ASTNode(new ASTMethodTypeOp($2, $5));
 			                                                $$->getDeclarator()->setStdCall($2->getDeclarator()->getStdCall());
                                                       }
|	ID LBRACKET array_bound RBRACKET                    { $$ = ASTNode(new ASTArrayTypeOp(ASTNode(new ASTDeclaratorId($1)), $3, false/*!inParam*/)); }
|	STAR declarator                                     { $$ = ASTNode(new ASTPtrTypeOp($2)); }
|	ADDR declarator                                     { $$ = ASTNode(new ASTRefTypeOp($2)); }
|	call_modifier declarator                            { $$ = $2; $$->getDeclarator()->setStdCall($1 == 1); }
;


maybe_param_seq:
	/*no params */          { $$.clear(); }
|  param_seq	            
;

param_seq:
	param                   { $$.clear(); if (!$1.type->isVoid()) $$.push_back($1);  }
| 	param_seq COMMA param   { $$ = $1; if (!$3.type->isVoid()) $$.push_back($3); }
;

param: 
	type_ref param_declarator      { $2->getDeclarator()->convert($1, $$); 
	                                 if ($$.type->isVoid() && $$.name != "") 
	                                 	throw RuntimeException("Illegal use of 'void' type with parameter '" + $$.name + "'");
	                               }
;

/*
	int; int *;   int (*)();  int fn(); int *f(); BUT NOT: int ();  int *(); 
*/

param_declarator:
	/*empty*/                          { $$ = ASTNode(new ASTDeclaratorId("")); }
|	declarator_arg
|	LPAREN param_declarator RPAREN LPAREN maybe_param_seq RPAREN    
 			                             { $$ = ASTNode(new ASTMethodTypeOp($2, $5)); 
 			                               $$->getDeclarator()->setStdCall($2->getDeclarator()->getStdCall()); 
                                      }
|	ID LBRACKET array_bound RBRACKET   { $$ = ASTNode(new ASTArrayTypeOp(ASTNode(new ASTDeclaratorId($1)), $3, true/*inParam*/)); }
|	STAR param_declarator              { $$ = ASTNode(new ASTPtrTypeOp($2)); }                              
|	ADDR param_declarator              { $$ = ASTNode(new ASTRefTypeOp($2)); }                              
|	call_modifier param_declarator     { $$ = $2; $$->getDeclarator()->setStdCall($1 == 1); }
;

declarator_arg:
	ID                                 { $$ = ASTNode(new ASTDeclaratorId($1)); }                               
|	ID LPAREN maybe_param_seq RPAREN   { $$ = ASTNode(new ASTMethodTypeOp(ASTNode(new ASTDeclaratorId($1)), $3)); }
;                                                   

array_bound:
   /*empty*/    { $$ = 0;}
|	NUM          { $$ = atoi($1.c_str());}
;

call_modifier:
	ATTR_CDECL                                                          { $$ = 0 /*!isStdCall*/; }
|	ATTR_STDCALL                                                        { $$ = 1 /*isStdCall*/; }
;

%%

void yy::CParser::error (const location_type& loc, const std::string& msg) {
	compiler.setErrorMessage(msg);
}





