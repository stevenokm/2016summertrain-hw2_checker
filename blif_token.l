%{
#include <cstdio>
#include <string>
#include "blif_grammar.hpp"
%}

%s IN_COMMENT
%option yylineno

%%

<INITIAL>{
	".model" { return MODEL; }
	".inputs" { return INPUTS; }
	".outputs" { return OUTPUTS; }
	".clock" { return CLOCK; }
	".end" { return END; }
	".names" { return NAMES; }
	[\ \t\\\n] ;
	[01\-]+ { yylval.token_string=strdup(yytext); return BOOL; }
	[0-9a-zA-Z_()$.<>/\[\]]+ { yylval.token_string=strdup(yytext); return ID; }
    /*\n { return LINEFEED; }*/
	#              BEGIN(IN_COMMENT);
}
<IN_COMMENT>{
	\n      BEGIN(INITIAL);
	. ;   // eat comment in chunks
}

%%
