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
#		- Jean Marie Lagniez  (lagniez@cril.fr)
#		- Daniel Le Berre 	  (leberre@cril.fr)
# 		- Tiago de Lima		  (delima@cril.fr)
#		- Valentin Montmirail (montmirail@cril.fr)
***************************************************************************************************/

#include "../include/PropositionalVariable.h"

/**
 * \file PropositionalVariable.cc
 * @brief Implementation of PropositionalVariable::methods which are not in the PropositionalVariable.h 
 * 
 * A Propositional Variable is an atom of the Formula.
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

/**
 * @brief The copy constructor from another PropositionalVariable
 *
 * We will just copy it as a Variable and have the same identifier.
 * 
 * @param[in] term the PropositionalVariable that we will copy.
 */
PropositionalVariable::PropositionalVariable(PropositionalVariable& term) : Variable(term) {

	negated = term.negated;

	setId(term.getId());	
}

/** 
 * @brief To construct a PropositionalVariable, we just need to construct a Variable.
 * 
 * @param[in] value the ID of the Variable in a string format.
 *
 */
PropositionalVariable::PropositionalVariable(char* value) : Variable(value) {

	/* What is send to this function is basically [p]{DIGIT}+, so we need to remove the 'p'. */
	unsigned int val = atoi(value+1);
	
	setId(val);

	/* If this new ID is bigger than the number of variable, then we discover a new atom. */
	if(Variable::getNbVars() <= val) {

		Variable::setNbVars(val);
	}
}

/**
 * @brief This method will check if the branch is {OK,KO,UNKNOWN} in the world world with the solution kripke.
 * 
 * @param[in] kripke the Kripke model which is (or not) the solution of the current formula.
 *
 * @param[in] world the world in which we are checking the satisfiability of the current PropositionalVariable.
 *
 * @return {OK,KO,UNKNOWN} according to the satisfiabiltiy of the current PropositionalVariable.
 */
unsigned int PropositionalVariable::checkBranch(Kripke & kripke, unsigned int world) {

	/*
	printf("We are checking : ");
	display();
	printf(" in w_%d\n",world);
	*/

	/* First, we want to check if the case [world][identifier] contains 1 in kripke. */
	char checkValue = 1;
	
	/* If we are checking negatively, we don't want to have an '1' but a '-1' */
	if(isNegated()) checkValue = -1;

	/* We get the stored value in the Kripke model at [world][identifier] */
	char storedValue = kripke.getAssignedValue(world,identifier);

	/* If it is matching, then it is OK */
	if(storedValue == checkValue) return OK;

	/* If we have the opposite, then we indicate it. */
	else if(-storedValue == checkValue) {
		    
		Term::setReason(this,NOT_VAR_FOUND_BUT_OPPOSITE,world);
		return KO;
	}

	/* If not, it means that storedValue is 0 and then we still don't know if it's OK or KO. So it's UNKNOWN. */
	Term::setReason(this,NOT_VAR_FOUND_NEITHER_OPPPOSITE,world);
	return UNKNOWN;
}