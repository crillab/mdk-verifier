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

#ifndef PROPOSITIONAL_VARIABLE_H_INCLUDED
#define PROPOSITIONAL_VARIABLE_H_INCLUDED

#include "Variable.h"

/**
 * \file PropositionalVariable.h
 * @brief Description of the class PropositionalVariable.
 * 
 * A Propositional Variable is an atom of the Formula.
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

/**
 * \class PropositionalVariable
 *
 * @brief A PropositionalVariable is just an Variable that contains an atom.
 */
class PropositionalVariable : public Variable {

public:  

      /**
       * @brief The copy constructor from another PropositionalVariable
       *
       * We will just copy it as a Variable and have the same identifier.
       * 
       * @param[in] term the PropositionalVariable that we will copy.
       */
  	PropositionalVariable(PropositionalVariable& term);

     /**
      * @brief To construct a PropositionalVariable, we just need to construct a Variable.
      * 
      * @param[in] value the ID of the Variable in a string format.
      *
      */
  	PropositionalVariable(char* value);

     /**
      * @brief To construct a PropositionalVariable, we just need to construct a Variable.
      * 
      * @param[in] value the ID of the Variable.
      *
      */ 
      PropositionalVariable(unsigned int value) : Variable(value) { setId(value); }

      /**
       * @brief To destroy a PropositionalVariable, we basically have nothing to do.
       */
      virtual ~PropositionalVariable () { }

      /**
       * @brief This method will return the ID of the propositional variable.
       *
       * @return the ID of the propositional variable.
       */
      unsigned int getId() { return identifier; }

      /**
       * @brief This method will set a new ID for the current propositional variable.
       * 
       * @param[in] id the new ID of the current propositional variable.
       */
      inline void setId(unsigned int id) { identifier = id;}

      /**
       * @brief This method will display the current PropositionalVariable.
       * 
       * @param[in] f the FILE* in which we want to display the PropositionalVariable.
       *
       * @note by default f is STDOUT, so without parameter, this function will just print on the screen.
       */      
      virtual void display(FILE* f = stdout) { if(this->isNegated()) {fprintf(f,"~");} fprintf(f,"p%d",this->identifier); }

      /**
       * @brief This method will recursively put the formula in the Negative Normal Form.
       * 
       * This means that only PropositionalVariable can be negated, the rest of the Formula can not.
       */      
      virtual void pushNegationInwards() { }

      /**
       * @brief This method will check if the branch is {OK,KO,UNKNOWN} in the world world with the solution kripke.
       * 
       * @param[in] kripke the Kripke model which is (or not) the solution of the current formula.
       *
       * @param[in] world the world in which we are checking the satisfiability of the current PropositionalVariable.
       *
       * @return {OK,KO,UNKNOWN} according to the satisfiabiltiy of the current PropositionalVariable.
       */
      virtual unsigned int checkBranch(Kripke & kripke, unsigned int world);

private:
    
      /** @brief The ID of the current PropositionalVariable */
	unsigned int identifier;

};

#endif