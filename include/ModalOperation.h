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


#ifndef MODAL_OPERATION_H_INCLUDED
#define MODAL_OPERATION_H_INCLUDED

#include "Operation.h"

/**
 * \file ModalOperation.h
 * @brief Description of the class ModalOperation.
 * 
 * An ModalOperation is an Operation that has only 1 child.
 * 
 * And has as operator {O_BOX, O_DIAMOND}
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

/**
 * \class ModalOperation
 *
 * @brief An ModalOperation is an Operation that has only 1 child.
 * 
 * And has as operator {O_BOX, O_DIAMOND}
 */
class ModalOperation : public Operation {

public:  

      /**
      * @brief To create a ModalOperation we need a Term and a modal operator.
      *
      * We will need also to call the constructor of a Variable.
      * 
      * @param[in] sub_formula the only child of our ModalOperation
      *
      * @param[in] op the modal operator of our current ModalOperation
      */
  	ModalOperation(Term* sub_formula, operator_t op);

      /**
       * @brief This method represents the recopy constructor.
       * 
       * If the ModalOperation that we are copying is negated, we will be also.
       *
       * And we will have the same children and same operator.
       */
  	ModalOperation(ModalOperation& modOp);

      /**
       * @brief To destroy a ModalOperation, we have nothing special to do.
       */
  	virtual ~ModalOperation () { }

      /**
       * @brief This method will display the operator and the sub-formula child.
       * 
       * @param[in] f the FILE* it which we print the constant.
       * 
       * @note by default, f is STDOUT and in that case, the constant is just display on the screen.
       */
      virtual void display(FILE* f = stdout);

      /**
       * @brief This method will affect the good value on each Term about:
       * 
       * At a current Term, is there a chain of modal operators, and how long it is ?
       *
       * @param[in] chainSize the current depth in a chain. Reinitiaziled to 0 everytime we 'break' a chain.
       */
      virtual void propagateInfoAboutModalChains(unsigned int & chainSize);

      /**
       * @brief This method will recursively propagates the association of equivalent boolean operators.
       *
       * (A ^ (B ^ (C ^ D))) will become (A ^ B ^ C ^ D)
       *
       * (A v (B v (C v D))) will become (A v B v C v D)
       * 
       * @note This method will just call it to the child.
       */ 
      virtual void boolAssociation();

      /**
       * @brief This method will recursively apply the Box Lifting rules on the formula.
       * 
       * (&lt;r1&gt;a v &lt;r1&gt;b) <=> &lt;r1&gt;(a v b) 
       *
       * ([r1]a ^ [r1]b) <=> [r1](a ^ b) 
       *
       * @note This method will just call it to the child.
       */
      virtual void boxLifting();

      /**
       * @brief This method will recursively put the formula in the Negative Normal Form.
       * 
       * This means that only PropositionalVariable can be negated, the rest of the Formula can not.
       */
      virtual void pushNegationInwards();

      /**
       * @brief This method will check if the branch is {OK,KO,UNKNOWN} in the world w with the solution kripke.
       * 
       * @param[in] kripke the Kripke model which is (or not) the solution of the current formula.

       * @param[in] w the world in which we are checking the satisfiability of the current ModalOperation.

       * @return {OK,KO,UNKNOWN} according to the satisfiabiltiy of the current ModalOperation.
       * 
       */
      virtual unsigned int checkBranch(Kripke & kripke, unsigned int w);

private:

      /** @brief This checker will become a Multi-Agent checker, so we need to already think about storing the ID of the agent. */
	unsigned int agent;

};

#endif