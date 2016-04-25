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
#   - Valentin Montmirail (valentin.montmirail@cril.fr)
#**************************************************************************************************/

%{   
  #include <stdlib.h>
  #include <list>
  #include "../include/Formula.h"
  
  
  // yylex exists
  extern int yylex();
  extern char yytext[];

  void yyerror(const char* msg);
%}

/*** Bison declarations ***/
%union
{
   bool         bval;
   int         agent;
   operator_t  opval;
   char        *sval;
   Term        *term;            
}

%token LROUND RROUND 

%left IFF
%left IMP
%left OR
%left AND
%right DIAMOND 
%right BOX 
%right NOT 

%token VALUE
%token IDENTIFIER

%type<bval>  VALUE
%type<sval>  IDENTIFIER

%type<term> Formula BooleanConstant BooleanFormula ModalFormula UnaryFormula PropositionalVariable
%type<opval> BinaryBoolOperator UnaryBoolOperator ModalOperator

%start Start

%%

/**********************************************************************
 *                                                                    *
 * Here are the rules to parse a formula from the starting point      *
 *                                                                    *
 **********************************************************************/

Start:  
| Formula  { 
    bool res = (Formula::getFormulaPtr())->setRoot($1); 
    if(res == false) {
      perror("Big problem, the ROOT is not attached !\n");
      exit(-1);
    }
}
;

/**********************************************************************/

Formula:  BooleanFormula               { $$ = $1; }
|         ModalFormula                 { $$ = $1; }
|         UnaryFormula                 { $$ = $1; }
|         LROUND Formula RROUND        { $$ = $2; }
;

/**********************************************************************/

BooleanConstant:   VALUE { 
  
  Term* t =  new BooleanConstant($1); 
  $$ = t;

};

/**********************************************************************/

PropositionalVariable:   IDENTIFIER { $$ = (Term*) new PropositionalVariable($1); }
;

/**********************************************************************/

BooleanFormula:   Formula BinaryBoolOperator Formula { 

                    Term* t = (Term*) new BooleanOperation($1, $3, $2);  /* can be (phi OR psi) or (phi AND psi) */ 
                    $$  = t;
                  }

|                 Formula IMP Formula {

                    $1->negate();
                    Term* t = (Term*) new BooleanOperation($1, $3, O_OR); 
                    $$ = t;

                  }

|                 PropositionalVariable IFF PropositionalVariable { 

                    PropositionalVariable* copyA = new PropositionalVariable(*(PropositionalVariable*)($1));
                    PropositionalVariable* copyB = new PropositionalVariable(*(PropositionalVariable*)($3));

                    copyA->negate(); 
                    copyB->negate();
                      
                    Term* Or1 = (Term*) new BooleanOperation(copyA, $3, O_OR); /* Or1 = (¬A v B) */
                    Term* Or2 = (Term*) new BooleanOperation(copyB, $1, O_OR); /* Or2 = (¬B v A) */
                      
                    Term* t = (Term*) new BooleanOperation(Or1, Or2, O_AND); /* We add : (Or1 AND OrB) */
                    $$ = t;
                  }
;

/**********************************************************************/

ModalFormula:   ModalOperator LROUND Formula RROUND {

                  Term* t = (Term*) new ModalOperation($3, $1);
                  $$ = t;
                }
|
                ModalOperator ModalFormula {

                  Term* t = (Term*) new ModalOperation($2, $1);
                  $$ = t;
                }
|
                ModalOperator UnaryFormula {

                  Term* t = (Term*) new ModalOperation($2, $1);
                  $$ = t;
                }
;

/**********************************************************************/

UnaryFormula:   BooleanConstant { $$ = $1; }
|
                PropositionalVariable { $$ = $1; }
|
                UnaryBoolOperator UnaryFormula {

                  if ($1 == O_NOT) {
                    ($2)->negate();   /* If ¬A, then we negate A before adding it */
                  }                   /* If  A, then we just add A without doing anything. */

                  $$ = $2; 
                }
|
                UnaryBoolOperator ModalFormula {

                  if ($1 == O_NOT) {
                    ($2)->negate();   /* If ¬A, then we negate A before adding it */
                  }                   /* If  A, then we just add A without doing anything. */

                  $$ = $2; 
                }
|
                UnaryBoolOperator LROUND Formula RROUND {

                  if ($1 == O_NOT) {
                    ($3)->negate();   /* If ¬A, then we negate A before adding it */
                  }                   /* If  A, then we just add A without doing anything. */

                  $$ = $3; 

                }
;

/**********************************************************************/

/**********************************************************************
 *                                                                    *
 * Here are the rules for the operators (Modal, Binary and Unary)
 *                                                                    *
 **********************************************************************/

ModalOperator:        BOX     { $$ = O_BOX; }
|                     DIAMOND { $$ = O_DIAMOND; }
;

BinaryBoolOperator:   AND     { $$ = O_AND; }
|                     OR      { $$ = O_OR; }
;

UnaryBoolOperator:    NOT     { $$ = O_NOT; }
;

/**********************************************************************/

%%

/**********************************************************************
 *                                                                    *
 * We reach the end, no rule is matching.                             *
 * The input file has a problem                                       *
 *                                                                    *
 **********************************************************************/

void yyerror(const char *msg) {

  printf("c PARSER: %s", msg);
  
  if (yytext[0] != 0) {
    
    printf(" near token '%s'\n", yytext);

  } else {

    printf("\n");
  }

  printf("c\n");
}

/**********************************************************************/

