/****************************************************************************************
* Copyright (c) 2016 Artois University
*
* This file is part of MDK-Verifier
*
* MDK-Verifier is free software: you can redistribute it and/or modify it 
* under the terms of the GNU General Public License as published by the Free Software Foundation, 
* either version 3 of the License, or (at your option) any later version.
*
* MDK-Verifier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with MDK-Verifier. 
* If not, see http://www.gnu.org/licenses/.
*
# Contributors:
#       - Jean Marie Lagniez  (lagniez@cril.fr)
#       - Daniel Le Berre     (leberre@cril.fr)
#       - Tiago de Lima       (delima@cril.fr)
#       - Valentin Montmirail (montmirail@cril.fr)
***************************************************************************************************/

#include "../include/Formula.h"

#include <unistd.h>
#include <sys/signal.h>


/**
 * \file Main.cc
 * @brief Entrance point of the modal logic K checker.
 * 
 * It reads an InToHyLo problem and a Kripke-CNF solution.
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 *
 * Main programs of the Modal Logic K Checker.
 */

/**
 * \fn int yyparse (void)
 *
 * @brief This function reads tokens, executes actions, and ultimately returns when it encounters end-of-input or an unrecoverable syntax error. 
 *
 * You can also write an action which directs yyparse to return immediately without reading further.
 *
 * @return  0 if parsing was successful (return is due to end-of-input).
 * @return  1 if parsing failed because of invalid input, i.e., input that contains a syntax error or that causes YYABORT to be invoked.
 * @return  2 if parsing failed due to memory exhaustion.
 */
extern int yyparse();

/**
 * @brief This file represents the INPUT send to Flex/Bison
 *
 * in our case, it will be assigned with the value of fopen(argv[1])
 */
extern FILE *yyin;


/** 
 * @brief The checking part is bounded by 300s
 */
#define LIMIT_ALARM 300

/**
 * \fn void catch_alarm(int sig)
 *
 * @brief This function will intercept all the SIGARLM send.
 *
 * It will make the program stops, saying that it was too long to check.
 *
 * @param[in] sig the number of the Signal send to this function.
 */
void catch_alarm(int sig) {

	printf("UNKNOWN: VERIFIER EXCEEDED TIME LIMIT (%d s)\n",LIMIT_ALARM);
	exit(0);
}

/**
 * \fn int main (int argc, char** argv)
 *
 * @brief This function is the entrance point of the checker.
 *
 * @param[in] argc the number of arguments send to the main.
 * @param[in] argv each argument in a char* format.
 *
 * @return EXIT_SUCCESS Normal behavior of the program.
 */
int main(int argc,char** argv) {


    clock_t begin,end;
    double timeParse;

	/* We match the catch_alarm function as our manager of SIGARLM signals. */
    signal(SIGALRM, catch_alarm);

    /* We get the sigleton instance of Formula as a more convinient name 'form'. */
    Formula* form = Formula::getFormulaPtr();

    /* We will for sure need a Kripke object to store the solution that we need to check. */
    Kripke solution;

    /* 
     * We need one and only argument.  
     * And this argument, should be the problem file. 
     */
    if(argc < 1) {
        fprintf(stderr,"The checker should be called as follow: \ncat solution | %s problem\nor %s problem < solution\n",argv[0],argv[0]);
        exit(-1);
    }
    
    /* By default; the formula should be as the argv[1] argument of the main. */
    if((yyin = fopen(argv[1],"r")) == NULL) {
    	fprintf(stderr,"Problem with opening the formula file\nVerify the path of %s",argv[1]);
    	exit(0);
    }
    
    /* 
     * We are calling the Bison parsing of the formula. 
     * If we crossed after this function, the formula is well parsed and in InToHyLo
     */
    yyparse();    

    /* It will display the formula before and after simplification. */
 	// form->display();

    /* 
     * We simplify the formula, so we put it in NNF, we make boolean operators as N-ary and we apply the rules of BoxLifting which are as follow:
     *
     * <r1>a v <r1>b <=> <r1>(a v b) 
     *
     * [r1]a ^ [r1]b <=> [r1](a ^ b) 
     * 
     * Then we will order children of each Term.
     */
    form->simplify();

    /**
     * We will add the information about how long is the next "modal operator" chain from each Term.
     * Here is some examples of how it's working:
     *
     * - (p1 v p2)    : Size = 0
     * - [](p1 v p2)  : Size = 1
     * - [][][][][]p3 : Size = 4  
     */
    unsigned int depth = 0;
    form->getRoot()->propagateInfoAboutModalChains(depth);

    /* it will display the formula before and after simplification. */
	// form->display();
	
    /* We start to parse the solution (that should came from STDIN). */
	solution.parseSolutionFile();

    alarm(LIMIT_ALARM);

	/* 
	 * We can now start to check the formula with the given solution. 
	 * There is only 3 exits possible: OK, KO and UNKNOWN.
	 */
	switch(form->checkFormula(solution)) {

		case KO:
			printf("ERROR: model is incorrect\n");
			printf("%s\n",Term::getReasonUnchecked().c_str());
			break;

		case OK:
			printf("OK %d\n",solution.getNbWorlds());
			break;

		default:
			printf("UNKNOWN: %s\n",Term::getReasonUnchecked().c_str());
			break;
	}

	/* Everything went well, the program finished, we indicate it to the Operating System. */
    return EXIT_SUCCESS;
}