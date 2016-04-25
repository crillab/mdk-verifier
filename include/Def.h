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

#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

#include "Kripke.h"

#include <map>

/**
 * \file Def.h
 * @brief Definition of enumerate types and also the preprocessor constants.
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

/**
 * @brief We declare an enumerate type for all the Operators possibles.
 */
enum operator_t {

  /** @brief ^ a logical and */
  O_AND = 0,

  /** @brief v a logical or */
  O_OR,

  /** @brief ~ a logical not */
  O_NOT,

  /** @brief [r1] a modal box */
  O_BOX,  

  /** @brief &lt;r1&gt; a modal diamond */
  O_DIAMOND,

  /** @brief in order to sort Term. */
  O_DOESNT_EXIST
}; 
  
/**
 * @brief In the Learning-Set, sometimes we don't know if we already check something. 
 * 
 * in that case, we are returning NOT_YET.
 *
 * @see Kripke
 */
#define NOT_YET 3 

/** 
 * @brief UNKNONN means that we didn't find what we want, nor the opposite while checking a sub-formula.
 */
#define UNKNOWN 2

/**
 * @brief OK means that we found what we wanted while checking a sub-formula. 
 */
#define OK      1

/**
 * @brief OK means that we found the opposite what we wanted while checking a sub-formula. 
 */
#define KO      0

/* --------------------------------------------------------------------------------------------------------- *
 *                                                                                                           *
 * We define error code to make a precise feedback if the check solution returns KO or UNKNOWN.              * 
 *                                                                                                           *
 * --------------------------------------------------------------------------------------------------------- */

/**
 * @brief This value is the neutral, we didn't find any reason of problem.
 */
#define NO_REASON_FOUND                  0

/**
 * @brief This value is a KO reason, we find the opposite of the propositional variable that we were looking for. 
 */
#define NOT_VAR_FOUND_BUT_OPPOSITE 		 1

/**
 * @brief This value is a UNKNOWN reason, we didn't find our propositional variable nor its opposite.
 */
#define NOT_VAR_FOUND_NEITHER_OPPPOSITE  2

/**
 * @brief This value is a UNKNOWN reason, We were looking for a sub-formula in a accessible world, but we didn't find this world.
 */
#define NO_EDGE_IS_GIVING_WHAT_WE_SEARCH 3

/**
 * @brief This value is a UNKNOWN reason, (UNKNOWN v KO v KO ... v KO) equals UNKNOWN
 */ 
#define UNKNOWN_OR_KO                    4

/**
 * @brief This value is a UNKNOWN reason, (UNKNOWN v UNKNOWN v ...  v UNKNOWN) equals UNKNOWN
 */  
#define UNKNOWN_OR_UNKNOWN               5

/**
 * @brief This value is a UNKNOWN reason, (UNKNOWN ^ UNKNOWN ^ ...  ^ UNKNOWN) equals UNKNOWN
 */  
#define UNKNOWN_AND_UNKNOWN              6

#endif 
