%{
    #include "attributes.h"
	#include "parser.tab.hpp"
    #include "output.hpp"
    using namespace output;
    int lex_error();
%}

%option yylineno
%option noyywrap

%%
void            {yylval.type_val = TYPE_ENUM_VOID; return VOID;}
int             {yylval.type_val = TYPE_ENUM_INT; return INT;}
byte            {yylval.type_val = TYPE_ENUM_BYTE; return BYTE;}
b               return B;
bool            {yylval.type_val = TYPE_ENUM_BOOL; return BOOL;}
struct          {return STRUCT;}
and             return AND;
or              return OR;
not             return NOT;
true            return TRUE;
false           return FALSE;
return          return RETURN;
if              return IF;
else            return ELSE;
while           return WHILE;
break           return BREAK;
continue        return CONTINUE;
";"               return SC;
","               return COMMA;
"."               return PERIOD;
"("               return LPAREN;
")"               return RPAREN;
"{"               return LBRACE;
"}"               return RBRACE;
"="               return ASSIGN;
"=="|"!="                 return EQ;
"<"|">"|"<="|">="     return RELATIONAL; 
"+"|"-"                   return ADD;
 "*"|"/"                  return MUL;
[a-zA-Z][a-zA-Z0-9]*        {yylval.string_val = new string(yytext); return ID;}
0|[1-9][0-9]*             {yylval.string_val = new string(yytext); return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\"   {yylval.string_val = new string(yytext); return STRING;}
"//"[^\r\n]*[\r|\n|\r\n]?     
(\r)|(\n)|(\r\n)
(" ")|(\t)       
.       lex_error();
%%

int lex_error()
{
    errorLex(yylineno);
    exit(0);
    return 0;
}