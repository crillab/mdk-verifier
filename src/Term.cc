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

#include "../include/PropositionalVariable.h"

Term* Term::nodeReason = NULL;

unsigned int Term::code = NO_REASON_FOUND;

unsigned int Term::worldReason = 0;

/**
 * \file Term.cc
 * @brief Implementation of Term::methods which are not in the Term.h
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

/**
 * @brief according to the code stored and the pointer of the Term*, this method will return an adapted message.
 * 
 * @return an adapted message according to the code and the Term* stored statically.
 */
 string Term::getReasonUnchecked() {

  string reason;

    switch(Term::code) {

        case NOT_VAR_FOUND_BUT_OPPOSITE:

            if(Term::nodeReason->isNegated()) reason.append("~");

            if(PropositionalVariable* p = dynamic_cast<PropositionalVariable*>(Term::nodeReason))
                reason.append("p" + to_string(p->getId()) + " is not in w_" + to_string(Term::worldReason) + " but its negation is !\n");

            break;  

        case NOT_VAR_FOUND_NEITHER_OPPPOSITE:

            if(Term::nodeReason->isNegated()) reason.append("~");

            if(PropositionalVariable* p = dynamic_cast<PropositionalVariable*>(Term::nodeReason))
                reason.append("p" + to_string(p->getId()) + "is not in w_" + to_string(Term::worldReason) + "(nor its negation) \n");
            break;

        case NO_EDGE_IS_GIVING_WHAT_WE_SEARCH:
            reason.append("No World accessible by w_" + to_string(Term::worldReason) + " contains what we want...\n");
            break;

        case UNKNOWN_OR_KO:
            reason.append("At least one part of an OR is wrong in w_" + to_string(Term::worldReason)+ " and the rest is UNKNOWN...\n");
            break;

        case UNKNOWN_OR_UNKNOWN:
            reason.append("All the branches of an OR are UNKNOWN in w_" + to_string(Term::worldReason)+ "...\n");
            break;

        case UNKNOWN_AND_UNKNOWN:
            reason.append("All the branches of an AND are UNKNOWN in w_" + to_string(Term::worldReason)+ "...\n");
            break;

        default:
            reason = "\n";
            break;

    }

    return reason;
}

/**
 * @brief This method will delete the i-th child of the current Term.
 * 
 * @param[in] i the index of the child that we want to delete
 *
 * @note this function is O(1).
 */
void Term::deleteChild(unsigned int i) {

  /* To delete the i-th, we just swap the i-th with the last one. */
  std::swap(children[i], children.back());

  /* and then we pop the last one out of the list. */
  children.pop_back();

}

/**
 * @brief This function will recursively sort all the children of every Term by the order as follow.
 *
 * - First PropositionalVariable and BooleanConstant
 * - Second BooleanOperation with a lot of children.
 * - Third  BooleanOperation with few children.
 * - Last   ModalOperator.
 */
void Term::orderChildren() {

    size_t size = children.size();

    if(size > 0) {

        sort(children.begin(), children.end());
        
        for(unsigned int i = 0; i < size; i++) children[i]->orderChildren();
    }
}

/**
 * @brief This function is allowing us to sort chlidren of every Term.
 *
 * - First PropositionalVariable and BooleanConstant
 * - Second BooleanOperation with a lot of children.
 * - Third  BooleanOperation with few children.
 * - Last   ModalOperator.
 *
 * @param[in] t1 The first Term that we want to compare with the Second
 *
 * @param[in] t2 The second Term that we want to compare with the First.
 *
 * @return true if t1 should be before t2, false otherwise.
 */
 bool operator<(Term & t1, Term & t2) {

    unsigned int nbChildrenT1 = t1.getNbChildren();
    unsigned int nbChildrenT2 = t2.getNbChildren();

    /* If one of them is a Variable, it should be first. */
    if(nbChildrenT1 == 0) return true;
    if(nbChildrenT2 == 0) return false;

    /* If one of them is a ModalOperator, it should be last. */
    if(nbChildrenT1 == 1) return false;
    if(nbChildrenT2 == 1) return true;

    /* If not, we just sort BooleanOperation with the one with the more children first. */
    return (nbChildrenT1 > nbChildrenT2);
 }
