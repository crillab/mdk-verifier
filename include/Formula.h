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


#ifndef FORMULA_H_INCLUDED
#define FORMULA_H_INCLUDED

#include "BooleanOperation.h"
#include "ModalOperation.h"

/**
 * \file Formula.h
 * @brief Description of the class Formula.
 * 
 * A formula has just a root pointer to a Term.
 * It represents a Tree of Terms.
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

/**
 * \class Formula
 *
 * @brief Class that represents a Tree of Term.
 * @note Singleton design Pattern.
 */
class Formula {

	public:  

  		/**
       * @brief To destroy a formula, we just need to delete the Root.
       * 
       * The destructor of a Term is destroying every child of it.
       */
  		virtual ~Formula () { delete root; }

      /**
       * @brief This method is returning the root of the Formula.
       * 
       * @return the Root of the Formula.
       */
  		Term* getRoot() { return root; }

      /**
       * @brief This method will set a new root for the Formula.
       * 
       * @param[in] term The new root of the formula.
       */
  		bool setRoot (Term* term);

      /**
       * @brief To display a formula, we just display the root of it.
       * 
       * @param[in] f the FILE* in which we will print the formula.
       * 
       * @note by default, f is STDOUT, so the method will display the formula on the screen.
       */
      void display(FILE* f = stdout) { instance->root->display(f); fprintf(f,"\n\n"); }

      /**
       * @brief This method is returning the Singleton instance as a Formula.
       *
       * @return the Singleton instance as a Formula.
       */
  		static Formula & getFormula() { return *instance; }

      /**
       * @brief This method is returning the Singleton instance as a Formula*
       *
       * @return the Singleton instance as a Formula*
       */
      static Formula* getFormulaPtr() { return instance; }

      /**
       * @brief We simplify the formula, so we put it in NNF, we make boolean operators as N-ary and we apply the rules of BoxLifting which are as follow:
       *
       * (&lt;r1&gt;a v &lt;r1&gt;b) <=> &lt;r1&gt;(a v b) 
       *
       * [r1]a ^ [r1]b <=> [r1](a ^ b) 
       *
       * Then we will order children of each Term.
       */
      void simplify();

      /**
       * @brief This method will check the formula according to a Kripke model.
       *
       * @param[in] kripke The Kripke model which is maybe a solution of the Formula.
       *
       * @return {OK,KO,UNKNOWN} according to the satisfiability of the Formula with the Kripke model.
       */
      unsigned int checkFormula(Kripke & kripke);

	private:

    /** @brief The constructor is private, and to construct a formula, we just put the root as NULL; */
    Formula() { root = NULL; }

    /** @brief a Formula is a tree, so we need a Root. */
    Term* root;

		/** @brief Static instance of formula (Singleton pattern) */
  	static Formula* instance;
  
};

#endif