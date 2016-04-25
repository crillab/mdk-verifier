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
* Contributors:
*   - Valentin Montmirail (valentin.montmirail@cril.fr)
***************************************************************************************************/

#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

#include "Term.h"

/**
 * \file Variable.h
 * @brief Description of the Abstract class Variable.
 * 
 * A Variable is a Term that can not have children.
 * This element could be a PropositionalVariable or a BooleanConstant.
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

/**
 * \class Variable
 *
 * @brief Abstract class of a Term that can NOT have children.
 *
 * This element could be a PropositionalVariable or a BooleanConstant.
 */
class Variable : public Term {

public:

	/**
     * @brief To construct an Variable, we just need to construct a Term.
     */
	Variable() : Term() { }

	/**
     * @brief To copy an Variable, we just need to copy a Term.
     */
    Variable(Variable& var) : Term(var) { }

    /**
     * @brief To construct a Variable, we just need to construct a Term.
     * 
     * @param[in] value the ID of the Variable.
     *
     * @note This function is doing nothing except making a Link between PropositionalVariable and Term.
     */
    Variable(unsigned int value) : Term() { }

    /**
     * @brief To construct a Variable, we just need to construct a Term.
     * 
     * @param[in] value the ID of the Variable in a string format.
     *
     * @note This function is doing nothing except making a Link between PropositionalVariable and Term.
     */
    Variable(char* value) : Term() { }

    /**
	 * @brief This method will set a new value on the number of propositional variable in the formula.
	 *
	 * @param[in] i the new number of propositional variables in the formula.
     */
    static void setNbVars(unsigned int i ) { nbVar = i; }

    /**
	 * @brief This method will return how many propositional variables there is the formula.
	 *
	 * @return the number of propositinal variables in the formula.
     */
    static unsigned int getNbVars() { return nbVar; }

private:

	/** @brief This static value will tell us, during the parsing, how many propositional variable there is in the formula */
    static unsigned int nbVar;

};

#endif