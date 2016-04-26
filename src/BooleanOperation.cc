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

#include "../include/BooleanOperation.h"

/**
 * \file BooleanOperation.cc
 * @brief Implementation of BooleanOperation::methods which are not in the BooleanOperation.h 
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

/**
 * @brief To create a BooleanOperation we need 2 Term and a boolean operator.
 *
 * We will need also to call the constructor of a Variable.
 *  
 * @param[in] left the left part of the binary operator
 *
 * @param[in] right the right part of the binary operator
 *
 * @param[in] op the boolean operator of our current BooleanOperation
 *
 * @note Every boolean operation is constructed as binary.
 */
BooleanOperation::BooleanOperation(Term* left, Term* right, operator_t op) {

	assert(op == O_AND || op == O_OR); 
	setOperator(op);

	addChild(left);
	addChild(right);

  negate(false);
}

/**
 * @brief This method represents the recopy constructor.
 * 
 * If the BooleanOperation that we are copying is negated, we will be also.
 * 
 * And we will have the same children and same operator.
 */
BooleanOperation::BooleanOperation(BooleanOperation& boolOp) : Operation(boolOp) {

  assert(boolOp.getOperator() == O_AND || boolOp.getOperator() == O_OR);
  setOperator(boolOp.getOperator());

  size_t size = boolOp.getChildren().size();

  for(unsigned int i = 0; i < size; i++) {

    addChild(boolOp.getChild(i));
  }

}

/**
 * @brief This method will recursively put the formula in the Negative Normal Form.
 * 
 * This means that only PropositionalVariable can be negated, the rest of the Formula can not.
 */
void BooleanOperation::pushNegationInwards() {

  /* If we are negated and we want to be NFF */
  if (isNegated()) {
    
    /* We need to swap the operator. */
    switch (getOperator()) {

      case O_AND: setOperator(O_OR);  break;

      case O_OR:  setOperator(O_AND); break;

      default: break;
    }
  }
  
  for(unsigned int i = 0; i < children.size(); i++) {
        
      /* If we are negated, we negate our children. */
      if(isNegated()) children[i]->negate();

      /* And we finally, recursively call the function on our children. */
      children[i]->pushNegationInwards();
  }

  /* Switch our negation to false, we are in NNF. */
  negated = false;
}

/**
 * @brief This method will display the operator and the sub-formulae children.
 * 
 * @param[in] f the FILE* it which we print the constant.
 * 
 * @note by default, f is STDOUT and in that case, the constant is just display on the screen.
 */    
void BooleanOperation::display(FILE* f) {

  if(isNegated()) {
    fprintf(f,"~");
  }

  fprintf(f,"(");

  size_t size = children.size();

  for(unsigned int i = 0; i < size; i++) {

    children[i]->display(f);

    /* If we still have a child, we need to separate it with our operator. */
    if(i+1 < size) {
      
      if(getOperator() == O_AND) fprintf(f," & ");

      else                       fprintf(f," | "); 
    }
  }

  fprintf(f,")");
}

/**
 * @brief This method will recursively propagates the association of equivalent boolean operators.
 * 
 * (A ^ (B ^ (C ^ D))) will become (A ^ B ^ C ^ D)
 * 
 * (A v (B v (C v D))) will become (A v B v C v D)
 */ 
void BooleanOperation::boolAssociation() {

  size_t sizeChild = 0;

  for(unsigned int i = 0; i < getChildren().size(); i++) {  

    children[i]->boolAssociation();

    if(Operation* op = dynamic_cast<Operation*>(children[i])) {

      if(getOperator() == op->getOperator()) {
          
        sizeChild = op->getChildren().size();

        for(unsigned int j = 0; j < sizeChild; j++) {  
          addChild(op->getChild(j));
        }

        deleteChild(i);
      }

    }
  }
}

/**
 * @brief This method will recursively apply the Box Lifting rules on the formula.
 * 
 * (&lt;r1&gt;a v &lt;r1&gt;b) <=> &lt;r1&gt;(a v b) 
 * 
 * ([r1]a ^ [r1]b) <=> [r1](a ^ b) 
 */
void BooleanOperation::boxLifting() {

  Operation* modOp  = NULL;
  Operation* modOp2 = NULL;

  for(unsigned int i = 0; i < getChildren().size(); i++) {  

    children[i]->boxLifting();

    if(modOp = dynamic_cast<Operation*>(children[i])) {

      if(getOperator() == O_AND && modOp->getOperator() == O_BOX) {

        /* We've got a []a AND ... We need to look if we can find a []b to merge. */
        for(unsigned int j = 0; j < getChildren().size(); j++) {

          if(j == i) continue;

          if(modOp2 = dynamic_cast<Operation*>(children[j])) {

            if(modOp2->getOperator() != O_BOX) continue;

            /* We've got here []a and []b <=> [](a and b) */
            BooleanOperation* newChild = new BooleanOperation(modOp->getChild(0),modOp2->getChild(0),O_AND);

            modOp->deleteChild(0);

            modOp->addChild(newChild);

            /* modOp2 should not exist anymore. */
            deleteChild(j);
            delete modOp2;
          }
        }

      } else if(getOperator() == O_OR && modOp->getOperator() == O_DIAMOND) {

        /* We've got a <>a OR ... We need to look if we can find a <>b to merge. */
        for(unsigned int j = 0; j < getChildren().size(); j++) {
          
          if(j == i) continue;

          if(modOp2 = dynamic_cast<Operation*>(children[j])) {

            if(modOp2->getOperator() != O_DIAMOND) continue;

            /* We've got here <>a or <>b <=> <>(a or b) */
            BooleanOperation* newChild = new BooleanOperation(modOp->getChild(0),modOp2->getChild(0),O_OR);

            modOp->deleteChild(0);
            modOp->addChild(newChild);

            /* modOp2 should not exist anymore. */
            deleteChild(j);
            delete modOp2;
          }

        }

      }
    }

  }
}


/** 
 * @brief This method will affect the good value on each Term about:
 * 
 * At a current Term, is there a chain of modal operators, and how long it is ?
 *
 * @param[in] chainSize the current depth in a chain. Reinitialized to 0 everytime we 'break' a chain.
 */
void BooleanOperation::propagateInfoAboutModalChains(unsigned int & chainSize) {

  /* We break for sure a chain if we are here, so depth = 0; let's try again */
  chainSize = 0;
  size_t size = getChildren().size();
  

  for(unsigned int i = 0; i < size; i++) {

      children[i]->propagateInfoAboutModalChains(chainSize);
  }
  
}

/**
 * @brief This method will check if the branch is {OK,KO,UNKNOWN} in the world world with the solution kripke.
 * 
 * @param[in] kripke the Kripke model which is (or not) the solution of the current formula.
 * 
 * @param[in] world the world in which we are checking the satisfiability of the current BooleanOperation.
 *
 * @return {OK,KO,UNKNOWN} according to the satisfiabiltiy of the current ModalOperation.
 * 
 */
unsigned int BooleanOperation::checkBranch(Kripke & kripke, unsigned int world) {

  unsigned int left = UNKNOWN;
  size_t nbChildren = children.size();
  unsigned int result = UNKNOWN;

  bool orKO = true;
  bool andOK = true;
  bool fullUnknown = true;

  /*
  printf("We are checking : ");
  display();
  printf(" in w_%d\n",world); 
  */
  
  if(getOperator() == O_OR) {

    /* We will check every child, until we find what we want. */
    for(unsigned int i = 0; i < nbChildren; i++) {
      
      /* We store the result of the i-th child */
      result = children[i]->checkBranch(kripke,world); 

      /* OK v whatever is OK, so we can return OK directly. */
      if(result == OK) return OK;

      /* We find a KO, so we know that we will not be full UNKNOWN. */
      else if(result == KO) fullUnknown = false;

      /* We find an UNKNOWN, so it can't be (KO v KO ... v KO) */
      else if(result == UNKNOWN) orKO = false;

    }

    /* If it was (KO v KO v ... KO) then we return KO */
    if(orKO) return KO;

    /* If it was not full UNKNOWN, we indicate it with our reason 'UNKONWN OR KO' */
    if(!fullUnknown) Term::setReason(this,UNKNOWN_OR_KO,world);

    /* If it was full UNKNOWN, we indicate it with our reason 'UNKOWN OR UNKNOWN' */
    else             Term::setReason(this,UNKNOWN_OR_UNKNOWN,world);

    /* We can now return UNKNOWN. */

  } else {

    /* We will check every child, until we find what we want. */
    for(unsigned int i =0 ; i < nbChildren; i++) {
      
      /* We store the result of the i-th child */
      result = children[i]->checkBranch(kripke,world); 

      /* KO ^ whatever is KO, so we can return KO directly. */
      if(result == KO) return KO;

      /* If we found an UNKNOWN, we know that we are not full OK. */
      else if(result == UNKNOWN) andOK = false;

    }

    /* if we are (OK ^ OK ^ OK .. ^ OK) then we return OK */
    if(andOK) return OK;

    /* If not, it was for sure an UNKNOWN somewhere. */
    Term::setReason(this,UNKNOWN_AND_UNKNOWN,world);
  }


  return UNKNOWN;
}