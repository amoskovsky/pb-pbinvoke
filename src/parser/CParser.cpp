/* A Bison parser, made by GNU Bison 2.1.  */

/* C++ Skeleton parser for LALR(1) parsing with Bison,
   Copyright (C) 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */


#include "CParser.tab.h"

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* A pseudo ostream that takes yydebug_ into account. */
# define YYCDEBUG							\
  for (bool yydebugcond_ = yydebug_; yydebugcond_; yydebugcond_ = false)	\
    (*yycdebug_)

/* Enable debugging if requested.  */
#if YYDEBUG

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << (Title) << ' ';			\
      yysymprint_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (0)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yyreduce_print_ (Rule);		\
} while (0)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (0)

#else /* !YYDEBUG */

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab

#if YYERROR_VERBOSE

/* Return YYSTR after stripping away unnecessary quotes and
   backslashes, so that it's suitable for yyerror.  The heuristic is
   that double-quoting is unnecessary unless the string contains an
   apostrophe, a comma, or backslash (other than backslash-backslash).
   YYSTR is taken from yytname.  */
std::string
yy::CParser::yytnamerr_ (const char *yystr)
{
  if (*yystr == '"')
    {
      std::string yyr = "";
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    yyr += *yyp;
	    break;

	  case '"':
	    return yyr;
	  }
    do_not_strip_quotes: ;
    }

  return yystr;
}

#endif

#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

void
yy::CParser::yysymprint_ (int yytype,
                         const semantic_type* yyvaluep, const location_type* yylocationp)
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;
  /* Backward compatibility, but should be removed eventually. */
  std::ostream& cdebug_ = *yycdebug_;
  (void) cdebug_;

  *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	     << ' ' << yytname_[yytype] << " ("
             << *yylocationp << ": ";
  switch (yytype)
    {
      default:
        break;
    }
  *yycdebug_ << ')';
}
#endif /* ! YYDEBUG */

void
yy::CParser::yydestruct_ (const char* yymsg,
                         int yytype, semantic_type* yyvaluep, location_type* yylocationp)
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yymsg;
  (void) yyvaluep;
  (void) yylocationp;

  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}

void
yy::CParser::yypop_ (unsigned int n)
{
  yystate_stack_.pop (n);
  yysemantic_stack_.pop (n);
  yylocation_stack_.pop (n);
}

std::ostream&
yy::CParser::debug_stream () const
{
  return *yycdebug_;
}

void
yy::CParser::set_debug_stream (std::ostream& o)
{
  yycdebug_ = &o;
}


yy::CParser::debug_level_type
yy::CParser::debug_level () const
{
  return yydebug_;
}

void
yy::CParser::set_debug_level (debug_level_type l)
{
  yydebug_ = l;
}


int
yy::CParser::parse ()
{
  /* Look-ahead and look-ahead in internal form.  */
  int yylooka;
  int yyilooka;

  /// Semantic value of the look-ahead.
  semantic_type yylval;
  /// Location of the look-ahead.
  location_type yylloc;
  /// The locations where the error started and ended.
  location yyerror_range[2];

  /// $$.
  semantic_type yyval;
  /// @$.
  location_type yyloc;

  int yyresult_;

  YYCDEBUG << "Starting parse" << std::endl;

  yynerrs_ = 0;
  yyerrstatus_ = 0;

  /* Start.  */
  yystate_ = 0;
  yylooka = yyempty_;


  /* Initialize the stacks.  The initial state will be pushed in
     yynewstate, since the latter expects the semantical and the
     location values to have been already stored, initialize these
     stacks with a primary value.  */
  yystate_stack_ = state_stack_type (0);
  yysemantic_stack_ = semantic_stack_type (0);
  yylocation_stack_ = location_stack_type (0);
  yysemantic_stack_.push (yylval);
  yylocation_stack_.push (yylloc);

  /* New state.  */
yynewstate:
  yystate_stack_.push (yystate_);
  YYCDEBUG << "Entering state " << yystate_ << std::endl;
  goto yybackup;

  /* Backup.  */
yybackup:

  /* Try to take a decision without look-ahead.  */
  yyn_ = yypact_[yystate_];
  if (yyn_ == yypact_ninf_)
    goto yydefault;

  /* Read a look-ahead token.  */
  if (yylooka == yyempty_)
    {
      YYCDEBUG << "Reading a token: ";
      yylooka = yylex (&yylval);
    }


  /* Convert token to internal form.  */
  if (yylooka <= yyeof_)
    {
      yylooka = yyilooka = yyeof_;
      YYCDEBUG << "Now at end of input." << std::endl;
    }
  else
    {
      yyilooka = yytranslate_ (yylooka);
      YY_SYMBOL_PRINT ("Next token is", yyilooka, &yylval, &yylloc);
    }

  /* If the proper action on seeing token ILOOKA_ is to reduce or to
     detect an error, take that action.  */
  yyn_ += yyilooka;
  if (yyn_ < 0 || yylast_ < yyn_ || yycheck_[yyn_] != yyilooka)
    goto yydefault;

  /* Reduce or error.  */
  yyn_ = yytable_[yyn_];
  if (yyn_ < 0)
    {
      if (yyn_ == yytable_ninf_)
	goto yyerrlab;
      else
	{
	  yyn_ = -yyn_;
	  goto yyreduce;
	}
    }
  else if (yyn_ == 0)
    goto yyerrlab;

  /* Accept?  */
  if (yyn_ == yyfinal_)
    goto yyacceptlab;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yyilooka, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yylooka != yyeof_)
    yylooka = yyempty_;

  yysemantic_stack_.push (yylval);
  yylocation_stack_.push (yylloc);

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus_)
    --yyerrstatus_;

  yystate_ = yyn_;
  goto yynewstate;

/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn_ = yydefact_[yystate_];
  if (yyn_ == 0)
    goto yyerrlab;
  goto yyreduce;

/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  yylen_ = yyr2_[yyn_];
  /* If LEN_ is nonzero, implement the default value of the action:
     `$$ = $1'.  Otherwise, use the top of the stack.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  */
  if (yylen_)
    yyval = yysemantic_stack_[yylen_ - 1];
  else
    yyval = yysemantic_stack_[0];

  {
    slice<location_type, location_stack_type> slice (yylocation_stack_, yylen_);
    YYLLOC_DEFAULT (yyloc, slice, yylen_);
  }
  YY_REDUCE_PRINT (yyn_);
  switch (yyn_)
    {
        case 14:

    { CoreInstance.setPack(atoi((yysemantic_stack_[1].text).c_str()));;}
    break;

  case 15:

    { 
                                                if ((yysemantic_stack_[1].text) != "pop") 
                                                	throw RuntimeException("#pragma pack: unexpected " + (yysemantic_stack_[1].text));
                                                CoreInstance.popPack();
                                              ;}
    break;

  case 16:

    { 
                                                if ((yysemantic_stack_[3].text) != "push") 
                                                	throw RuntimeException("#pragma pack: unexpected " + (yysemantic_stack_[3].text));
                                                CoreInstance.pushPack(atoi((yysemantic_stack_[1].text).c_str()));
                                              ;}
    break;

  case 17:

    { (yysemantic_stack_[0].node)->getNodeList()->nameDef((yysemantic_stack_[1].type), compiler); ;}
    break;

  case 18:

    { (yysemantic_stack_[0].node)->getNodeList()->typeDef((yysemantic_stack_[1].type), compiler); ;}
    break;

  case 20:

    { (yyval.type) = (yysemantic_stack_[0].node)->getTypeRef(); ;}
    break;

  case 23:

    { (yyval.node) = (yysemantic_stack_[0].node); /*ignore declspec yet*/ ;}
    break;

  case 24:

    { (yyval.node) = ASTNode(new ASTInternalType(compiler)); (yyval.node)->setTypeAttr((yysemantic_stack_[0].ival)); ;}
    break;

  case 25:

    { (yyval.node) = (yysemantic_stack_[1].node); (yyval.node)->setTypeAttr((yysemantic_stack_[0].ival)); ;}
    break;

  case 26:

    { (yyval.node) = (yysemantic_stack_[1].node); /*ignore declspec yet*/ ;}
    break;

  case 27:

    { (yyval.ival) = ATTR_SIGNED; ;}
    break;

  case 28:

    { (yyval.ival) = ATTR_UNSIGNED; ;}
    break;

  case 29:

    { (yyval.ival) = ATTR_CHAR; ;}
    break;

  case 30:

    { (yyval.ival) = ATTR_INT8; ;}
    break;

  case 31:

    { (yyval.ival) = ATTR_WCHAR_T; ;}
    break;

  case 32:

    { (yyval.ival) = ATTR_SHORT; ;}
    break;

  case 33:

    { (yyval.ival) = ATTR_INT16; ;}
    break;

  case 34:

    { (yyval.ival) = ATTR_INT; ;}
    break;

  case 35:

    { (yyval.ival) = ATTR_INT; ;}
    break;

  case 36:

    { (yyval.ival) = ATTR_INT64; ;}
    break;

  case 37:

    { (yyval.ival) = ATTR_INT; ;}
    break;

  case 38:

    { (yyval.ival) = ATTR_LONGLONG; ;}
    break;

  case 39:

    { (yyval.ival) = ATTR_DOUBLE; ;}
    break;

  case 40:

    { (yyval.ival) = ATTR_FLOAT; ;}
    break;

  case 41:

    { (yyval.ival) = ATTR_VOID; ;}
    break;

  case 42:

    { (yyval.node) = ASTNode(new ASTUserType((yysemantic_stack_[0].text), compiler)); ;}
    break;

  case 43:

    { (yyval.node) = (yysemantic_stack_[0].node); ;}
    break;

  case 44:

    { (yyval.node) = (yysemantic_stack_[1].node); /*ignore declspec yet*/;}
    break;

  case 45:

    { (yyval.node) = ASTNode(new ASTUserType((yysemantic_stack_[0].text), compiler)); ;}
    break;

  case 46:

    { (yyval.node) = (yysemantic_stack_[3].node); (yyval.node)->getStructType()->completeStruct((yysemantic_stack_[4].ival), (yysemantic_stack_[1].items)); ;}
    break;

  case 49:

    { (yyval.ival) = TC_STRUCT_TYPE; ;}
    break;

  case 50:

    { (yyval.ival) = TC_UNION_TYPE; ;}
    break;

  case 51:

    { (yyval.node) = ASTNode(new ASTStructType("", compiler)); ;}
    break;

  case 52:

    { (yyval.node) = ASTNode(new ASTStructType((yysemantic_stack_[0].text), compiler)); ;}
    break;

  case 53:

    { (yyval.node) = ASTNode(new ASTStructType("", compiler)); ;}
    break;

  case 54:

    { (yyval.node) = ASTNode(new ASTStructType((yysemantic_stack_[0].text), compiler)); ;}
    break;

  case 55:

    { (yyval.items).clear(); ;}
    break;

  case 56:

    { (yyval.items) = (yysemantic_stack_[2].items); (yyval.items).insert((yyval.items).end(), (yysemantic_stack_[1].items).begin(), (yysemantic_stack_[1].items).end()); ;}
    break;

  case 57:

    { (yysemantic_stack_[0].node)->getNodeList()->nameDef((yysemantic_stack_[1].type), (yyval.items), compiler); ;}
    break;

  case 58:

    { (yyval.node) = ASTNode(new ASTNodeList()); ;}
    break;

  case 60:

    { (yyval.node) = ASTNode(new ASTNodeList((yysemantic_stack_[0].node))); ;}
    break;

  case 61:

    { (yyval.node) = (yysemantic_stack_[2].node); (yyval.node)->getNodeList()->list.push_back((yysemantic_stack_[0].node)); ;}
    break;

  case 62:

    { (yyval.node) = (yysemantic_stack_[0].node); ;}
    break;

  case 63:

    { (yyval.node) = ASTNode(new ASTMethodTypeOp((yysemantic_stack_[4].node), (yysemantic_stack_[1].items)));
 			                                                (yyval.node)->getDeclarator()->setStdCall((yysemantic_stack_[4].node)->getDeclarator()->getStdCall());
                                                       ;}
    break;

  case 64:

    { (yyval.node) = ASTNode(new ASTArrayTypeOp(ASTNode(new ASTDeclaratorId((yysemantic_stack_[3].text))), (yysemantic_stack_[1].ival), false/*!inParam*/)); ;}
    break;

  case 65:

    { (yyval.node) = ASTNode(new ASTPtrTypeOp((yysemantic_stack_[0].node))); ;}
    break;

  case 66:

    { (yyval.node) = ASTNode(new ASTRefTypeOp((yysemantic_stack_[0].node))); ;}
    break;

  case 67:

    { (yyval.node) = (yysemantic_stack_[0].node); (yyval.node)->getDeclarator()->setStdCall((yysemantic_stack_[1].ival) == 1); ;}
    break;

  case 68:

    { (yyval.items).clear(); ;}
    break;

  case 70:

    { (yyval.items).clear(); if (!(yysemantic_stack_[0].namedType).type->isVoid()) (yyval.items).push_back((yysemantic_stack_[0].namedType));  ;}
    break;

  case 71:

    { (yyval.items) = (yysemantic_stack_[2].items); if (!(yysemantic_stack_[0].namedType).type->isVoid()) (yyval.items).push_back((yysemantic_stack_[0].namedType)); ;}
    break;

  case 72:

    { (yysemantic_stack_[0].node)->getDeclarator()->convert((yysemantic_stack_[1].type), (yyval.namedType)); 
	                                 if ((yyval.namedType).type->isVoid() && (yyval.namedType).name != "") 
	                                 	throw RuntimeException("Illegal use of 'void' type with parameter '" + (yyval.namedType).name + "'");
	                               ;}
    break;

  case 73:

    { (yyval.node) = ASTNode(new ASTDeclaratorId("")); ;}
    break;

  case 75:

    { (yyval.node) = ASTNode(new ASTMethodTypeOp((yysemantic_stack_[4].node), (yysemantic_stack_[1].items))); 
 			                               (yyval.node)->getDeclarator()->setStdCall((yysemantic_stack_[4].node)->getDeclarator()->getStdCall()); 
                                      ;}
    break;

  case 76:

    { (yyval.node) = ASTNode(new ASTArrayTypeOp(ASTNode(new ASTDeclaratorId((yysemantic_stack_[3].text))), (yysemantic_stack_[1].ival), true/*inParam*/)); ;}
    break;

  case 77:

    { (yyval.node) = ASTNode(new ASTPtrTypeOp((yysemantic_stack_[0].node))); ;}
    break;

  case 78:

    { (yyval.node) = ASTNode(new ASTRefTypeOp((yysemantic_stack_[0].node))); ;}
    break;

  case 79:

    { (yyval.node) = (yysemantic_stack_[0].node); (yyval.node)->getDeclarator()->setStdCall((yysemantic_stack_[1].ival) == 1); ;}
    break;

  case 80:

    { (yyval.node) = ASTNode(new ASTDeclaratorId((yysemantic_stack_[0].text))); ;}
    break;

  case 81:

    { (yyval.node) = ASTNode(new ASTMethodTypeOp(ASTNode(new ASTDeclaratorId((yysemantic_stack_[3].text))), (yysemantic_stack_[1].items))); ;}
    break;

  case 82:

    { (yyval.ival) = 0;;}
    break;

  case 83:

    { (yyval.ival) = atoi((yysemantic_stack_[0].text).c_str());;}
    break;

  case 84:

    { (yyval.ival) = 0 /*!isStdCall*/; ;}
    break;

  case 85:

    { (yyval.ival) = 1 /*isStdCall*/; ;}
    break;


      default: break;
    }

/* Line 676 of lalr1.cc.  */


  yypop_ (yylen_);

  YY_STACK_PRINT ();

  yysemantic_stack_.push (yyval);
  yylocation_stack_.push (yyloc);

  /* Shift the result of the reduction.  */
  yyn_ = yyr1_[yyn_];
  yystate_ = yypgoto_[yyn_ - yyntokens_] + yystate_stack_[0];
  if (0 <= yystate_ && yystate_ <= yylast_
      && yycheck_[yystate_] == yystate_stack_[0])
    yystate_ = yytable_[yystate_];
  else
    yystate_ = yydefgoto_[yyn_ - yyntokens_];
  goto yynewstate;

/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus_)
    {
      ++yynerrs_;
      error (yylloc, yysyntax_error_ (YYERROR_VERBOSE_IF (yyilooka)));
    }

  yyerror_range[0] = yylloc;
  if (yyerrstatus_ == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yylooka <= yyeof_)
        {
	  /* Return failure if at end of input.  */
	  if (yylooka == yyeof_)
	    YYABORT;
        }
      else
        {
          yydestruct_ ("Error: discarding", yyilooka, &yylval, &yylloc);
          yylooka = yyempty_;
        }
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (false)
    goto yyerrorlab;

  yyerror_range[0] = yylocation_stack_[yylen_ - 1];
  yypop_ (yylen_);
  yystate_ = yystate_stack_[0];
  goto yyerrlab1;

/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn_ = yypact_[yystate_];
      if (yyn_ != yypact_ninf_)
	{
	  yyn_ += yyterror_;
	  if (0 <= yyn_ && yyn_ <= yylast_ && yycheck_[yyn_] == yyterror_)
	    {
	      yyn_ = yytable_[yyn_];
	      if (0 < yyn_)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yystate_stack_.height () == 1)
	YYABORT;

      yyerror_range[0] = yylocation_stack_[0];
      yydestruct_ ("Error: popping",
                   yystos_[yystate_],
                   &yysemantic_stack_[0], &yylocation_stack_[0]);
      yypop_ ();
      yystate_ = yystate_stack_[0];
      YY_STACK_PRINT ();
    }

  if (yyn_ == yyfinal_)
    goto yyacceptlab;

  yyerror_range[1] = yylloc;
  // Using YYLLOC is tempting, but would change the location of
  // the look-ahead.  YYLOC is available though.
  YYLLOC_DEFAULT (yyloc, yyerror_range - 1, 2);
  yysemantic_stack_.push (yylval);
  yylocation_stack_.push (yyloc);

  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos_[yyn_],
		   &yysemantic_stack_[0], &yylocation_stack_[0]);

  yystate_ = yyn_;
  goto yynewstate;

  /* Accept.  */
yyacceptlab:
  yyresult_ = 0;
  goto yyreturn;

  /* Abort.  */
yyabortlab:
  yyresult_ = 1;
  goto yyreturn;

yyreturn:
  if (yylooka != yyeof_ && yylooka != yyempty_)
    yydestruct_ ("Cleanup: discarding lookahead", yyilooka, &yylval, &yylloc);

  while (yystate_stack_.height () != 1)
    {
      yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
      yypop_ ();
    }

  return yyresult_;
}

// Generate an error message.
std::string
yy::CParser::yysyntax_error_ (YYERROR_VERBOSE_IF (int tok))
{
  std::string res;
#if YYERROR_VERBOSE
  yyn_ = yypact_[yystate_];
  if (yypact_ninf_ < yyn_ && yyn_ < yylast_)
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  */
      int yyxbegin = yyn_ < 0 ? -yyn_ : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = yylast_ - yyn_;
      int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
      int count = 0;
      for (int x = yyxbegin; x < yyxend; ++x)
        if (yycheck_[x + yyn_] == x && x != yyterror_)
          ++count;

      // FIXME: This method of building the message is not compatible
      // with internationalization.  It should work like yacc.c does it.
      // That is, first build a string that looks like this:
      // "syntax error, unexpected %s or %s or %s"
      // Then, invoke YY_ on this string.
      // Finally, use the string as a format to output
      // yytname_[tok], etc.
      // Until this gets fixed, this message appears in English only.
      res = "syntax error, unexpected ";
      res += yytnamerr_ (yytname_[tok]);
      if (count < 5)
        {
          count = 0;
          for (int x = yyxbegin; x < yyxend; ++x)
            if (yycheck_[x + yyn_] == x && x != yyterror_)
              {
                res += (!count++) ? ", expecting " : " or ";
                res += yytnamerr_ (yytname_[x]);
              }
        }
    }
  else
#endif
    res = YY_("syntax error");
  return res;
}


/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
const signed char yy::CParser::yypact_ninf_ = -86;
const short int
yy::CParser::yypact_[] =
{
      16,   -86,   134,   -86,   -86,   -30,   -17,   -86,   -86,   -86,
     -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,   -86,
     -86,   -86,   -86,   134,    22,    -9,    47,   -86,   -86,   -86,
     -86,   -86,   -86,    73,   -86,   151,   -86,    35,   -86,   134,
       5,    73,    50,     8,    73,   -86,    47,   -86,   -28,   -86,
     -86,    73,    73,    73,   -86,    26,   -86,   -86,    73,   -86,
     -86,   -86,   -86,    13,    85,    57,   -86,    63,    61,    64,
     -86,   -86,   134,    98,    69,   -86,   -86,    73,   -86,   -86,
     -86,   101,   -86,   103,   -86,   -86,    75,    76,    86,   -86,
     -86,    81,    88,   -86,   108,   104,   105,   -27,    75,    75,
      75,   -86,   -86,    75,   -86,   134,   -86,   134,   -86,    73,
     111,   107,   -86,    98,   113,   -86,   -86,   -86,   -86,   130,
      26,   -86,   -86,   145,   112,   -86,   -86,   134,   150,   -86
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
const unsigned char
yy::CParser::yydefact_[] =
{
       0,    42,     0,    49,    50,     0,     0,    28,    27,    30,
      33,    34,    36,    39,    40,    29,    31,    35,    37,    38,
      32,    41,     8,     0,     0,     2,     3,     5,     9,    13,
      11,    10,    12,    58,    20,    21,    24,    22,    43,     0,
      51,    58,     0,     0,    58,     1,     7,     4,    80,    84,
      85,     0,     0,     0,    17,    59,    60,    62,     0,    25,
      26,    44,    23,    45,    53,     0,    18,     0,     0,     0,
      19,     6,    68,    82,     0,    65,    66,     0,    67,    54,
      55,     0,    47,     0,    15,    14,    73,     0,    69,    70,
      83,     0,     0,    61,     0,     0,     0,    80,    73,    73,
      73,    72,    74,    73,    81,     0,    64,    68,    46,     0,
       0,     0,    16,    82,     0,    77,    78,    79,    71,     0,
      57,    56,    48,     0,     0,    63,    76,    68,     0,    75
};

/* YYPGOTO[NTERM-NUM].  */
const short int
yy::CParser::yypgoto_[] =
{
     -86,   -86,   -86,   161,   -19,   -86,   -86,   -86,   -86,   -86,
       0,   148,   -86,   153,   -86,   -86,    14,   -86,   -86,   -86,
     -86,    45,    80,    32,   -50,   -86,    87,    82,   -85,    77,
       1
};

/* YYDEFGOTO[NTERM-NUM].  */
const signed char
yy::CParser::yydefgoto_[] =
{
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      86,    34,    35,    36,    37,    38,    39,    40,    65,    94,
     110,    54,    55,    56,    87,    88,    89,   101,    57,    91,
      58
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.  */
const signed char yy::CParser::yytable_ninf_ = -53;
const short int
yy::CParser::yytable_[] =
{
      33,   102,    41,    42,     6,    72,    72,    47,    63,    73,
     113,    68,    69,   102,   102,   102,    43,     5,   102,     1,
      22,     2,    45,    44,     3,     4,    33,    71,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    33,     5,   -52,    60,
       1,    61,     2,    67,    64,     3,     4,   119,    77,     5,
      23,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    48,   128,    97,    49,
      50,    49,    50,    74,    75,    76,    66,   103,    79,    70,
      78,    23,    80,    83,   109,    84,    81,    82,    85,   103,
     103,   103,    90,    92,   103,    95,    51,    96,    98,    93,
     104,     1,    52,    53,    99,   100,     3,     4,   105,   106,
       5,   107,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     1,   111,   112,
     121,   122,     3,     4,   108,   127,     5,   124,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     5,   125,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     114,   115,   116,   126,   129,   117,    46,    62,    59,   120,
     123,     0,   118
};

/* YYCHECK.  */
const signed char
yy::CParser::yycheck_[] =
{
       0,    86,     2,    33,    13,    33,    33,    26,     3,    37,
      37,     3,     4,    98,    99,   100,    33,    12,   103,     3,
      29,     5,     0,    23,     8,     9,    26,    46,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    46,    12,    35,    35,
       3,    37,     5,     3,    40,     8,     9,   107,    32,    12,
      44,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     3,   127,     3,     6,
       7,     6,     7,    51,    52,    53,    41,    86,     3,    44,
      58,    44,    35,    32,    94,    34,    33,    34,    34,    98,
      99,   100,     4,    34,   103,     4,    33,     4,    33,    77,
      34,     3,    39,    40,    39,    40,     8,     9,    32,    38,
      12,    33,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,     3,    34,    34,
      29,    34,     8,     9,    36,    33,    12,    34,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    12,    34,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      98,    99,   100,    38,    34,   103,    25,    39,    35,   109,
     113,    -1,   105
};

/* STOS_[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
const unsigned char
yy::CParser::yystos_[] =
{
       0,     3,     5,     8,     9,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    44,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    57,    33,    33,    57,     0,    50,    51,     3,     6,
       7,    33,    39,    40,    68,    69,    70,    75,    77,    60,
      63,    63,    58,     3,    63,    65,    68,     3,     3,     4,
      68,    51,    33,    37,    70,    70,    70,    32,    70,     3,
      35,    33,    34,    32,    34,    34,    57,    71,    72,    73,
       4,    76,    34,    70,    66,     4,     4,     3,    33,    39,
      40,    74,    75,    77,    34,    32,    38,    33,    36,    57,
      67,    34,    34,    37,    74,    74,    74,    74,    73,    71,
      69,    29,    34,    76,    34,    34,    38,    33,    71,    34
};

#if YYDEBUG
/* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
   to YYLEX-NUM.  */
const unsigned short int
yy::CParser::yytoken_number_[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
const unsigned char
yy::CParser::yyr1_[] =
{
       0,    47,    48,    49,    49,    49,    49,    49,    50,    50,
      51,    51,    51,    52,    53,    53,    53,    54,    55,    56,
      57,    58,    58,    58,    59,    59,    59,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    61,    61,    61,    62,    62,    63,    63,    64,
      64,    65,    65,    65,    65,    66,    66,    67,    68,    68,
      69,    69,    70,    70,    70,    70,    70,    70,    71,    71,
      72,    72,    73,    74,    74,    74,    74,    74,    74,    74,
      75,    75,    76,    76,    77,    77
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
const unsigned char
yy::CParser::yyr2_[] =
{
       0,     2,     1,     1,     2,     1,     3,     2,     1,     1,
       1,     1,     1,     1,     4,     4,     6,     2,     3,     3,
       1,     1,     1,     2,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     5,     4,     7,     1,
       1,     0,     1,     1,     2,     0,     3,     2,     0,     1,
       1,     3,     1,     6,     4,     2,     2,     2,     0,     1,
       1,     3,     2,     0,     1,     6,     4,     2,     2,     2,
       1,     4,     0,     1,     1,     1
};

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at \a yyntokens_, nonterminals. */
const char*
const yy::CParser::yytname_[] =
{
  "$end", "error", "$undefined", "ID", "NUM", "T_TYPEDEF", "ATTR_CDECL",
  "ATTR_STDCALL", "T_STRUCT", "T_UNION", "T_ENUM", "ATTR_CONST",
  "ATTR_DECLSPEC", "T_PRAGMA_PACK", "T_UNSIGNED", "T_SIGNED", "T_INT8",
  "T_INT16", "T_INT32", "T_INT64", "T_DOUBLE", "T_FLOAT", "T_CHAR",
  "T_WCHAR_T", "T_INT", "T_LONG", "T_LONGLONG", "T_SHORT", "T_VOID",
  "SEMICOLON", "DBLCOLON", "ASSIGN", "COMMA", "LPAREN", "RPAREN", "LCURL",
  "RCURL", "LBRACKET", "RBRACKET", "STAR", "ADDR", "PARAM_SEQ", "ATTR_PRE",
  "ATTR_POST", "TEST_RULE", "LOW", "HIGH", "$accept", "prog", "decl_seq",
  "sep", "decl", "pragma", "pragma_pack", "namedef", "typedef", "namedef2",
  "type_ref", "attributed_type", "internal_type", "attr", "user_type",
  "struct_type", "declspec", "struct", "struct_tag", "var_decl_seq",
  "var_decl", "maybe_declarator_seq", "declarator_seq", "declarator",
  "maybe_param_seq", "param_seq", "param", "param_declarator",
  "declarator_arg", "array_bound", "call_modifier", 0
};
#endif

#if YYDEBUG
/* YYRHS -- A `-1'-separated list of the rules' RHS. */
const yy::CParser::rhs_number_type
yy::CParser::yyrhs_[] =
{
      48,     0,    -1,    49,    -1,    50,    -1,    50,    51,    -1,
      51,    -1,    49,    50,    51,    -1,    49,    50,    -1,    29,
      -1,    52,    -1,    55,    -1,    54,    -1,    56,    -1,    53,
      -1,    13,    33,     4,    34,    -1,    13,    33,     3,    34,
      -1,    13,    33,     3,    32,     4,    34,    -1,    57,    68,
      -1,     5,    57,    68,    -1,    44,    57,    68,    -1,    58,
      -1,    59,    -1,    61,    -1,    63,    58,    -1,    60,    -1,
      59,    60,    -1,    59,    63,    -1,    15,    -1,    14,    -1,
      22,    -1,    16,    -1,    23,    -1,    27,    -1,    17,    -1,
      18,    -1,    24,    -1,    19,    -1,    25,    -1,    26,    -1,
      20,    -1,    21,    -1,    28,    -1,     3,    -1,    62,    -1,
      61,    63,    -1,    64,     3,    -1,    64,    65,    35,    66,
      36,    -1,    12,    33,     3,    34,    -1,    12,    33,     3,
      33,     4,    34,    34,    -1,     8,    -1,     9,    -1,    -1,
       3,    -1,    63,    -1,    63,     3,    -1,    -1,    66,    67,
      29,    -1,    57,    69,    -1,    -1,    69,    -1,    70,    -1,
      69,    32,    70,    -1,    75,    -1,    33,    70,    34,    33,
      71,    34,    -1,     3,    37,    76,    38,    -1,    39,    70,
      -1,    40,    70,    -1,    77,    70,    -1,    -1,    72,    -1,
      73,    -1,    72,    32,    73,    -1,    57,    74,    -1,    -1,
      75,    -1,    33,    74,    34,    33,    71,    34,    -1,     3,
      37,    76,    38,    -1,    39,    74,    -1,    40,    74,    -1,
      77,    74,    -1,     3,    -1,     3,    33,    71,    34,    -1,
      -1,     4,    -1,     6,    -1,     7,    -1
};

/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
const unsigned char
yy::CParser::yyprhs_[] =
{
       0,     0,     3,     5,     7,    10,    12,    16,    19,    21,
      23,    25,    27,    29,    31,    36,    41,    48,    51,    55,
      59,    61,    63,    65,    68,    70,    73,    76,    78,    80,
      82,    84,    86,    88,    90,    92,    94,    96,    98,   100,
     102,   104,   106,   108,   110,   113,   116,   122,   127,   135,
     137,   139,   140,   142,   144,   147,   148,   152,   155,   156,
     158,   160,   164,   166,   173,   178,   181,   184,   187,   188,
     190,   192,   196,   199,   200,   202,   209,   214,   217,   220,
     223,   225,   230,   231,   233,   235
};

/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
const unsigned short int
yy::CParser::yyrline_[] =
{
       0,    63,    63,    67,    68,    69,    70,    71,    75,    76,
      80,    81,    82,    86,    89,    90,    95,   103,   108,   113,
     119,   123,   124,   125,   129,   130,   131,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   153,   154,   155,   159,   160,   164,   165,   170,
     171,   175,   176,   177,   178,   183,   184,   188,   193,   194,
     199,   200,   205,   206,   210,   211,   212,   213,   218,   219,
     223,   224,   228,   239,   240,   241,   245,   246,   247,   248,
     252,   253,   257,   258,   262,   263
};

// Print the state stack on the debug stream.
void
yy::CParser::yystack_print_ ()
{
  *yycdebug_ << "Stack now";
  for (state_stack_type::const_iterator i = yystate_stack_.begin ();
       i != yystate_stack_.end (); ++i)
    *yycdebug_ << ' ' << *i;
  *yycdebug_ << std::endl;
}

// Report on the debug stream that the rule \a yyrule is going to be reduced.
void
yy::CParser::yyreduce_print_ (int yyrule)
{
  unsigned int yylno = yyrline_[yyrule];
  /* Print the symbols being reduced, and their result.  */
  *yycdebug_ << "Reducing stack by rule " << yyn_ - 1
             << " (line " << yylno << "), ";
  for (unsigned char i = yyprhs_[yyn_];
       0 <= yyrhs_[i]; ++i)
    *yycdebug_ << yytname_[yyrhs_[i]] << ' ';
  *yycdebug_ << "-> " << yytname_[yyr1_[yyn_]] << std::endl;
}
#endif // YYDEBUG

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
yy::CParser::token_number_type
yy::CParser::yytranslate_ (int token)
{
  static
  const token_number_type
  translate_table[] =
  {
         0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
  };
  if ((unsigned int) token <= yyuser_token_number_max_)
    return translate_table[token];
  else
    return yyundef_token_;
}

const int yy::CParser::yyeof_ = 0;
const int yy::CParser::yylast_ = 192;
const int yy::CParser::yynnts_ = 31;
const int yy::CParser::yyempty_ = -2;
const int yy::CParser::yyfinal_ = 45;
const int yy::CParser::yyterror_ = 1;
const int yy::CParser::yyerrcode_ = 256;
const int yy::CParser::yyntokens_ = 47;

const unsigned int yy::CParser::yyuser_token_number_max_ = 301;
const yy::CParser::token_number_type yy::CParser::yyundef_token_ = 2;




void yy::CParser::error (const location_type& loc, const std::string& msg) {
	compiler.setErrorMessage(msg);
}






