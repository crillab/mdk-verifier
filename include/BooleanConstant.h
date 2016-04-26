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
#           - Jean Marie Lagniez  (lagniez@cril.fr)
#           - Daniel Le Berre         (leberre@cril.fr)
#           - Tiago de Lima           (delima@cril.fr)
#           - Valentin Montmirail (montmirail@cril.fr)
***************************************************************************************************/

#ifndef BOOLEAN_CONSTANT_H_INCLUDED
#define BOOLEAN_CONSTANT_H_INCLUDED

#include "Variable.h"

/**
 * \file BooleanConstant.h
 * @brief Description of the class BooleanConstant
 * 
 * A BooleanConstant is just a boolean value True or False as a Term.
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */


/**
 * \class BooleanConstant
 *
 * @brief A BooleanConstant is just Variable that contains only {true, false}
 */
class BooleanConstant : public Variable {

	public:  

      /**
       * @brief To construct a constant, we need its value.
       *
       * @param[in] val the value of the constant (True or False) 
       */
  	BooleanConstant(bool val) { value = val; }

      /**
       * @brief To copy a constant, we just need to copy a Variable + its value.
       *
       * @param[in] boolConstant the Boolean constant that we want to copy.
       */
      BooleanConstant(BooleanConstant& boolConstant) : Variable(boolConstant) { this->value = boolConstant.value; }

  	/**
       * @brief To destroy a constant, we have nothing to do.
       */
  	virtual ~BooleanConstant () { }

      /**
       * @brief To negate a constant, we just switch its value.
       *  
       * There is no need to store "~True" if we can just store "False"
       */
      inline void negateConstant() { value = !value; }

      /**
       * @brief This method will return the value of the constant.
       * 
       * @return the value of the constant.
       */
  	inline bool getValue() { return value; }

      /**
       * @brief To put in NNF a constant, we just need to switch its value. 
       */
      virtual void pushNegationInwards() { if(isNegated()) { value = !value; negate(false); }  }

      /**
       * @brief This method will display "true" or "false" according to the value of the constant.
       * 
       * @param[in] f the FILE* it which we print the constant.
       * 
       * @note by default, f is STDOUT and in that case, the constant is just display on the screen.
       */
      virtual void display(FILE* f = stdout);

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
      virtual unsigned int checkBranch(Kripke & kripke, unsigned int world);

	private:

    /** @brief Represents the constant as a primitive value. */
		bool value;

	};

#endif