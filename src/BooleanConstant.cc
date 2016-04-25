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
*		- Valentin Montmirail (valentin.montmirail@cril.fr)
***************************************************************************************************/

#include "../include/BooleanConstant.h"

/**
 * \file BooleanConstant.cc
 * @brief Implementation of BooleanConstant::methods which are not in the BooleanConstant.h 
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */


/**
 * @brief This method will verify if the constant is true in the world.
 * 
 * Basically, every world are logical. So we just need to return OK if the constant is True.
 * 
 * and KO is the constant is false, No need to check anything.
 *
 * @param[in] kripke the Kripke model which is maybe a solution for the Formula.
 * 
 * @param[in] world the world in which we perform the check. Irrelevant here;
 */
unsigned int BooleanConstant::checkBranch(Kripke & kripke, unsigned int world) {

	if(value) return OK;

	return KO;
}


/**
 * @brief This method will display "true" or "false" according to the value of the constant.
 * 
 * @param[in] f the FILE* it which we print the constant.
 * 
 * @note by default, f is STDOUT and in that case, the constant is just display on the screen.
 */
void BooleanConstant::display(FILE* f) {

	if(value) fprintf(f,"true");
	
	else  fprintf(f,"false");
}