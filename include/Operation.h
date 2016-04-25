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

#ifndef OPERATION_H_INCLUDED
#define OPERATION_H_INCLUDED

#include "BooleanConstant.h"
#include "PropositionalVariable.h"

/**
 * \file Operation.h
 * @brief Description of the Abstract class Operation.
 * 
 * An Operation is a Term that can have children.
 * This element could be a ModalOperation or a BooleanOperation.
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

/**
 * \class Operation
 *
 * @brief Abstract class of a Term that can have children.
 *
 * This element could be a ModalOperation, a BooleanOperation.
 */
class Operation : public Term {

public:  

      /**
       * @brief To construct an Operation, we just need to construct a Term.
       */
      Operation() : Term() { }

  		/**
       * @brief To recopy an Operator, we just need to recopy it as a Term.
       */
  		Operation (Operation& term) : Term(term) { }

  		/**
       * @brief To destroy an Operation, we have nothing special to do.
       */
  		virtual ~Operation() { }

      /**
       * @brief This method will return the operator of the Operation.
       *
       * @return the operator of the Operation.
       */
  		inline operator_t getOperator() { return op; }

      /**
       * @brief This method will set a new operation for the current Operation.
       *
       * @param[in] _op the new operator of our Operation.
       */
  		inline void setOperator(operator_t _op) { op = _op; }

private:

		  /** @brief The operator for the operation */
  		operator_t op;

};

#endif