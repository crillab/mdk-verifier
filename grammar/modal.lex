/****************************************************************************************
# Copyright (c) 2016 Artois University
#
# This file is part of MDK-Verifier
#
# MDK-Verifier is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free Software Foundation, 
# either version 3 of the License, or (at your option) any later version.
#
# MDK-Verifier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with MDK-Verifier. 
# If not, see http://www.gnu.org/licenses/.
#
# Contributors:
#		- Valentin Montmirail (valentin.montmirail@cril.fr)
#**************************************************************************************************/

%{
#include "../include/Formula.h"
#include "modal.tab.h"
#include <string.h>
%}


/*** Options ***/
%option noyywrap
%option never-interactive

/*** Regular name definitions ***/
DIGIT                   [0-9]
LETTER                  [a-zA-Z]
WHITE                   [ \t\r\n]+

/*** Rules ***/
%%

{WHITE}                      { } /* Spaces */

"#"[^\n]*\n                  { } /* Comment line */

"true"                       { yylval.bval = true; return VALUE; } 
"false"                      { yylval.bval = false; return VALUE; } 

[p]{DIGIT}+                  { yylval.sval = strdup(yytext); return IDENTIFIER; } /* Identifiers (propositional variables) */

"~"                          { return NOT; } /* Operators (boolean and modal) */
"&"                          { return AND; }
"|"                          { return OR; }
"->"                         { return IMP; }
"<->"                        { return IFF; }
"[r"{DIGIT}+"]"              { yylval.agent = IDENTIFIER; return BOX; }
"<r"{DIGIT}+">"              { yylval.agent = IDENTIFIER; return DIAMOND; }

"begin" 					 { return LROUND; }
"("                          { return LROUND; }

"end"						 { return RROUND; }
")"                          { return RROUND; }

'\t'                         { printf("LEXER: warning: '%s'\n", yytext); } 

.                            { printf("LEXER: unrecognized token '%s'\n", yytext); 
							   exit(-1);
							 } /* Unrecognized text */

<<EOF>>                      { return EOF; } /* End of file */


%%
/*** Code inserted at the and of the file ***/

