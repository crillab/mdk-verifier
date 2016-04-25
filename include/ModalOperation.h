/****************************************************************************************
# Copyright (c) 2015-20XX, Valentin Montmirail
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
# OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/


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