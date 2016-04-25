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

#ifndef TERM_H_INCLUDED
#define TERM_H_INCLUDED

#include "Def.h"

/**
 * \file Term.h
 * @brief Description of the Abstract class Term.
 * 
 * a Term is an element in a modal logic formula (stored as a Tree).
 * This element could be a ModalOperation, a BooleanOperation, a PropositionalVariable or a BooleanConstant.
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

/**
 * \class Term
 *
 * @brief Abstract class of an element in a modal logic formula (stored as a Tree).
 *
 * This element could be a ModalOperation, a BooleanOperation, a PropositionalVariable or a BooleanConstant.
 */
class Term {

	public:  

		  /**
       * @brief This method represents the default constructor.
       * 
       * It will just initialize the list of children empty and say that the element is positive.
       */
  		Term () { negated = false; modalTermChainSize = 0; }

  		/**
       * @brief This method represents the recopy constructor.
       * 
       * If the Term that we are copying is negated, we will be also. And they will have the same list of children.
       */
  		Term (Term& term) { negated = term.negated; modalTermChainSize = term.negated; }

  		/**
       * @brief This method represents the destructor of the class. It will just clear the list of children.
       * 
       * In that way, every child will commit a suicide by calling his destructor. 
       */
  		virtual ~Term () { children.clear(); }

      /**
       * @brief This method will change the negation of the Term.
       * 
       * A negative Term will become positive. 
       * A positive Term will become negative.
       */
  		inline void negate() { negated = !negated; }

      /**
       * @brief This method will assign the negation of the Term.
       * 
       * @param[in] val the new value for the negation of the Term.
       */
      inline void negate(bool val) { negated = val; }


      /**
       * @brief This method will return the negation of the Term.
       * 
       * @return true if the Term is negative, false otherwise.
       */
  		inline bool isNegated() { return negated; }

      /**
       * @brief This method will add the parameter as a new child of the Term.
       * 
       * @param[in] child the new child of the current Term.
       */
  		virtual void addChild(Term* child) { children.push_back(child); child->setFather(this); }

      /**
       * @brief This method will return the i-th child of the Term.
       * 
       * @param[in] i the index of the child in the list of children.

       * @return the i-th child of the current Term.
       */
  		Term* getChild(unsigned int i) { return children[i]; }
		
      /**
       * @brief This method will return all the children as a vector
       * 
       * @return all the children of the Term as a vector.
       */
      vector<Term*> getChildren() { return children; }

      /**
       * @brief This method will affect the good value on each Term about:
       * 
       * At a current Term, is there a chain of modal operators, and how long it is ?
       *
       * @param[in] chainSize the current depth in a chain. Reinitialized to 0 everytime we 'break' a chain.
       */
      virtual void propagateInfoAboutModalChains(unsigned int & chainSize) { }

      /**
       * @brief This method will set the Father of the current Term.
       * 
       * @param[in] t the new father of the Term.
       */
      void setFather(Term* t) { father = t; }

      /**
       * @brief This method will return a pointer to the Father of the Term.
       * 
       * @return the father of the Term.
       */
      Term* getFather() { return father; }

      /**
       * @brief This function will return the number of children stored.
       * 
       * @return the number of children stored;
       */
      unsigned int getNbChildren() const { return children.size(); }

      /**
       * @brief This method will delete the i-th child of the current Term.
       * 
       * @param[in] i the index of the child that we want to delete
       *
       * @note this function is O(1).
       */
      void deleteChild(unsigned int i);

      /**
       * @brief This method will display the current Term and also its children
       * 
       * @param[in] f the FILE* in which we want to display the Term.
       *
       * @note by default f is STDOUT, so without parameter, this function will just print on the screen.

       * @note this function is virtual, every element in a modal formula can be displayed.
       */
      virtual void display(FILE* f = stdout) { }

      /**
       * @brief This method will check if the branch is {OK,KO,UNKNOWN} in the world w with the solution kripke.
       * 
       * @param[in] kripke the Kripke model which is (or not) the solution of the current formula.

       * @param[in] w the world in which we are checking the satisfiability of the current Term.

       * @return {OK,KO,UNKNOWN} according to the satisfiabiltiy of the current Term.
       * 
       * @note Term is abstract, so by default it is returning UNKNOWN, but it should normally never be call.
       */
      virtual unsigned int checkBranch(Kripke & kripke, unsigned int w) { return UNKNOWN; }

      /**
       * @brief This method will recursively propagates the association of equivalent boolean operators.
       *
       * (A ^ (B ^ (C ^ D))) will become (A ^ B ^ C ^ D)
       *
       * (A v (B v (C v D))) will become (A v B v C v D)
       * 
       * @note This method is virtual but it is only "doing" something in ModalOperation and BooleanOperation.
       */ 
      virtual void boolAssociation() { }

      /**
       * @brief This method will recursively apply the Box Lifting rules on the formula.
       * 
       * (&lt;r1&gt;a v &lt;r1&gt;b) <=> &lt;r1&gt;(a v b) 
       *
       * ([r1]a ^ [r1]b) <=> [r1](a ^ b) 
       *
       * @note This method is virtual but it is only "doing" something in ModalOperation and BooleanOperation.
       */
      virtual void boxLifting() { }
      
      /**
       * @brief This method will recursively put the formula in the Negative Normal Form.
       * 
       * This means that only PropositionalVariable can be negated, the rest of the Formula can not.
       */
      virtual void pushNegationInwards() { }

      /**
       * @brief This function will set a new modal term chain size for the current Term.
       * 
       * @param newSize the new size of the Chain.
       */
      void setModalTermChainSize(unsigned int newSize) { modalTermChainSize = newSize; }

      /**
       * @brief This function will return the size of the modal term chain 
       * 
       * @return the size of the modal term chain 
       */
      unsigned int getModalTermChainSize() { return modalTermChainSize; }

      /**
       * @brief This function will recursively sort all the children of every Term by the order as follow.
       *
       * - First PropositionalVariable and BooleanConstant
       * - Second BooleanOperation with a lot of children.
       * - Third  BooleanOperation with few children.
       * - Last   ModalOperator.
       */
      virtual void orderChildren();

      /**
       * @brief according to the code stored and the pointer of the Term*, this method will return an adapted message.
       * 
       * @return an adapted message according to the code and the Term* stored statically.
       */
      static string getReasonUnchecked();

      /**
       * @brief This method will set the Checking Code, a pointer to the Term causing this reason and the world in which it happened.
       * 
       * @param[in] t a pointer to the Term that cause the call of Term::setReason
       * 
       * @param[in] c the Checking code set as the reason @see Def.h
       * 
       * @param[in] w the world in which the call of Term::setReason happened.
       */
      static void setReason(Term* t, unsigned int c,unsigned int w) { nodeReason = t; code = c; worldReason = w; }

	protected: 

    /**
     * @brief Every Term can have child, but obivously it makes sense only if Operation has children.
     */
    vector<Term*> children; 

    /**
     * @brief a Term can be negative or positive and we need to know it, thanks to this attribute.
     */
		bool negated;

    /**
     * @brief a Term can access its father to transfer him some informations. (Like how deep is the following modal Depth)
     */
    Term* father;

    /**
     * @brief For each Term, we need to know how long is the chain of modal operators.
     *
     * Here is some examples of how it's working:
     *
     * - (p1 v p2)    : Size = 0
     * - [](p1 v p2)  : Size = 1
     * - [][][][][]p3 : Size = 4 
     */
    unsigned int modalTermChainSize;

  private:

    /**
     * @brief To return a reason if the checking part returns {KO,UNKNOWN} we need to have a pointer to the Term causing the {KO,UNKNOWN} 
     */
    static Term* nodeReason;

    /**
     * @brief the Checking code @see Def.h to understand the reason and return in 'getReasonUnchecked()' an adapted message.
     */
    static unsigned int code;

    /** 
     * @brief The world in which the reason of {KO,UNKNOWN} appeared.
     */
    static unsigned int worldReason;
	};


 /**
  * @brief This function is allowing us to sort chlidren of every Term.
  *
  * We basically put PropositionalVariable first, then Modal, then Boolean.
  */
  extern bool operator<(Term & t1, Term & t2);

#endif  