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

#include "../include/ModalOperation.h"

#define SHRINK_CHAIN_OF_WORLDS true

/**
 * \file ModalOperation.cc
 * @brief Implementation of ModalOperation::methods which are not in the ModalOperation.h 
 * 
 * An ModalOperation is an Operation that has only 1 child.
 * 
 * And has as operator {O_BOX, O_DIAMOND}
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

/*
 * To create a ModalOperation we need a Term and a modal operator.
 *
 * We will need also to call the constructor of a Variable.
 * 
 * sub_formula the only child of our ModalOperation
 *
 * op the modal operator of our current ModalOperation
 */
ModalOperation::ModalOperation(Term* sub_formula, operator_t op) {

	assert(op == O_DIAMOND || op == O_BOX);

	/* We set the operator if it is a Modal Operator. */
	setOperator(op);

	Term::addChild(sub_formula);

	/* For now, we don't deal with multi-agent, so we are agent 1 for sure. */
	agent = 1;
}

/*
 * This method represents the recopy constructor.
 * 
 * If the ModalOperation that we are copying is negated, we will be also.
 *
 * And we will have the same children and same operator.
 */
ModalOperation::ModalOperation(ModalOperation& modOp) : Operation(modOp)  {

	
}

/*
 * This method will recursively put the formula in the Negative Normal Form.
 * 
 * This means that only PropositionalVariable can be negated, the rest of the Formula can not.
 */
void ModalOperation::pushNegationInwards() {

  	assert(getOperator() == O_DIAMOND || getOperator() == O_BOX);

  	/* If we are negated and we want to be NFF */
  	if (isNegated()) {

    	/* We need to swap the operator. */
    	switch (getOperator()) {

      		case O_BOX: setOperator(O_DIAMOND); break;
      
      		case O_DIAMOND: setOperator(O_BOX); break;

      		default: break;
    	} 
  	}
      
    /* If we are negated, we negate our children. */
    if(isNegated()) children[0]->negate();
       
    /* And we finally, recursively call the function on our child. */
    children[0]->pushNegationInwards();
 
    /* Switch our negation to false, we are in NNF. */
    negated = false;
}

/** 
 * @brief This method will affect the good value on each Term about:
 * 
 * At a current Term, is there a chain of modal operators, and how long it is ?
 *
 * @param[in] chainSize the current depth in a chain. Reinitiaziled to 0 everytime we 'break' a chain.
 */
void ModalOperation::propagateInfoAboutModalChains(unsigned int & chainSize) {

	/* We first go as far as possible in the depth. */
	children[0]->propagateInfoAboutModalChains(chainSize);

	ModalOperation* mod_op = dynamic_cast<ModalOperation*>(children[0]);

	if(mod_op != NULL && (mod_op->getOperator() == getOperator())) {	

		/* Here is the backTrack part, our child was chainSize * our operator, we so have a chain size of (chainSize+1). */
		/* And our parent will have a chain size of (our size+1) ... */
		this->setModalTermChainSize(++chainSize);

	} else {

		/* Here we are the end of the chain, the child was not a modal. So we are [r]something. */
		chainSize = 1;

		/* We set the chainSize at 1, and the backTrack will increase chainSize to all our parent which have the same operator as us. */
		this->setModalTermChainSize(chainSize);
	}	
}


/*
 * This method will recursively apply the Box Lifting rules on the formula.
 * 
 * (<r1>a v <r1>b) <=> <r1>(a v b)
 *
 * ([r1]a ^ [r1]b) <=> [r1](a ^ b) 
 *
 * This method will just call it to the child.
 */
void ModalOperation::boxLifting() {


	/* We just propagate the boxLifting function to our child. */	
	children[0]->boxLifting();
}

/*
 * This method will display the operator and the sub-formula child.
 * 
 * f the FILE* it which we print the constant.
 * 
 * by default, f is STDOUT and in that case, the constant is just display on the screen.
 */
void ModalOperation::display(FILE* f) {

	if(isNegated()) {
		fprintf(f,"~");
	}

	if(getOperator() == O_DIAMOND) fprintf(f,"<r%d>",agent);
	else                           fprintf(f,"[r%d]",agent);

	children[0]->display(f);
}


/*
 * This method will recursively propagates the association of equivalent boolean operators.
 *
 * (A ^ (B ^ (C ^ D))) will become (A ^ B ^ C ^ D)
 *
 * (A v (B v (C v D))) will become (A v B v C v D)
 * 
 * This method will just call it to the child.
 */ 
void ModalOperation::boolAssociation() {

	children[0]->boolAssociation();
}

/*
 * This method will check if the branch is {OK,KO,UNKNOWN} in the world w with the solution kripke.
 * 
 * kripke the Kripke model which is (or not) the solution of the current formula.
 *
 * w the world in which we are checking the satisfiability of the current ModalOperation.
 *
 * return {OK,KO,UNKNOWN} according to the satisfiabiltiy of the current ModalOperation.
 * 
 */
unsigned int ModalOperation::checkBranch(Kripke & kripke, unsigned int w) {

	/* We get all the worlds that we are connected with. */
	vector<unsigned int>* worldConnected = kripke.getConnectedWorlds(w);
	if(worldConnected == NULL) worldConnected = new vector<unsigned int>;

	/* We need to know how many there is. */
	unsigned int size = worldConnected->size();
	Term* check = this;	
	unsigned int res = UNKNOWN;

	// printf("We are checking : ");
	// display();
	// printf(" in w_%d\n",w);
		


	/** shrink of chain of worlds no need to () -> () -> (p1) let's jump directly to the important part. */

	if(SHRINK_CHAIN_OF_WORLDS) {

  		unsigned int sizeOfChain = getModalTermChainSize();
  		unsigned int w_tmp = w;
  			
  		while(sizeOfChain > 1 && size == 1) {

  			w_tmp = kripke.nextWorld(w_tmp);
		
			size = kripke.getNbConnectedWorlds(w_tmp);

			sizeOfChain--;

			check = check->getChild(0);
	
  		}	
  	
  		w = w_tmp;
		worldConnected = kripke.getConnectedWorlds(w_tmp);		

		// printf("We are checking : ");
		// check->display();
		// printf(" in w_%d\n",w);	
  	}
  	
	/* We are checking <r1>PHI_w so we just need to find 1 world, connected to w, where PHI is true. */
	if(getOperator() == O_DIAMOND) {

		for(unsigned int i = 0; i < size; i++) {
 			
 			/* We are checking PHI in w_i */
			res = check->getChild(0)->checkBranch(kripke,(*worldConnected)[i]);

			/* If it is OK, then <r1>PHI is w is also OK. */
			if(res == OK) {
				return OK;
			}
		}

		/* 
		 * We look through every connected world, and we didn't find an OK world. 
		 * So we don't know and we indicate it with a reason. 
		 */
		Term::setReason(this,NO_EDGE_IS_GIVING_WHAT_WE_SEARCH,w);
	
	} else {

		/* We are checking [r1]PHI_w so we just need every worlds connected to w with PHI true. */

		for(unsigned int i = 0; i < size; i++) {

			/* We are checking PHI in w_i */
			res = check->getChild(0)->checkBranch(kripke,(*worldConnected)[i]);
			
			/* If it is KO, then [r1]PHI_w is KO */
			if(res == KO) {
				return KO;
			}

			/* If it is UNKNOWN, then [r1]PHI_w is UNKNOWN */
			if(res == UNKNOWN) {
				return UNKNOWN;
			}

		}	

		/* Every world were OK, so it is OK. */
		return OK;
	}
	
	return UNKNOWN;
}