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
#   - Jean Marie Lagniez  (lagniez@cril.fr)
#   - Daniel Le Berre     (leberre@cril.fr)
#     - Tiago de Lima     (delima@cril.fr)
#   - Valentin Montmirail (montmirail@cril.fr)
***************************************************************************************************/

#include "../include/Formula.h"

/**
 * \file Formula.cc
 * @brief  Implementation of Formula::methods which are not in the Formula.h
 * 
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

Formula *Formula::instance = new Formula();

/** 
 * @brief This method will set a new root for the Formula.
 * 
 * @param[in] term The new root of the formula.
 */
 bool Formula::setRoot (Term* term) {

  /* If there is already a root, we don't change it. */
  if (root) return false;

  root = term;

  return true;
 }

/**
 * @brief We simplify the formula, so we put it in NNF, we make boolean operators as N-ary and we apply the rules of BoxLifting which are as follow:
 *
 * (&lt;r1&gt;a v &lt;r1&gt;b) <=> &lt;r1&gt;(a v b) 
 *
 * [r1]a ^ [r1]b <=> [r1](a ^ b)
 *
 * Then we will order children of each Term. 
 */
void Formula::simplify() {
  
  if(root != NULL) {

    /* This method will put the formula in NNF */
    root->pushNegationInwards();
    
    /* This method will make boolean operators as N-ary. */
    root->boolAssociation();
      
    /* This method will apply the BoxLifting rules on every part of the formula. */
    root->boxLifting();

    /* This method will sort children according if they are easy to check or not. */
    root->orderChildren();
  }

}

/**
 * @brief This method will check the formula according to a Kripke model.
 *
 * @param[in] kripke The Kripke model which is maybe a solution of the Formula.
 *
 * @return {OK,KO,UNKNOWN} according to the satisfiability of the Formula with the Kripke model.
 */
unsigned int Formula::checkFormula(Kripke & kripke) {

  if(root != NULL) {

    /* To check a Formula, it means to check the Root in w0, the only world for sure existing. */
    return root->checkBranch(kripke,0);
  }

  /* If there is no ROOT, it is difficult to check a formula. */
  return UNKNOWN;
}