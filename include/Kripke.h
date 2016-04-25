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

#ifndef KRIPKE_H_INCLUDED
#define KRIPKE_H_INCLUDED

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <signal.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#include "Def.h"

class Term;

/**
 * \file Kripke.h
 * @brief Description of the class Kripke.
 * 
 * A Kripke model represents the expected solution for a Modal Logic K problem.
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

using namespace std;

/**
 * \class Kripke
 *
 * @brief A Kripke model represents the expected solution for a Modal Logic K problem.
 */
class Kripke {

public:

    /** 
     * @brief The default constructor will just initiliaze everything to 0 and NULL. 
     */
    Kripke() { num_worlds = 0; num_variables = 0; edges = NULL; }

    /** 
     * @brief This method is the destructor, it will just delete all the edges stored. 
     */
	~Kripke() { if(edges != NULL) { for(unsigned int i =0; i < num_worlds; i++) delete edges[i]; free(edges); edges = NULL; } }

    /**
     * @brief This method will assigned the atom 'value' in the world 'world'.
     * 
     * @param[in] value the atom that we want to assign (value < 0) if we want to atom negatively.
     *
     * @param[in] world the world in which the atom is assigned.
     */
    void assignValue(int value, unsigned int world);

    /**
     * @brief This method will add an edge between w1 --> w2.
     * 
     * @param[in] w1 the starting world
     * 
     * @param[in] w2 the arrival world.
     */
    void addEdge(unsigned int w1, unsigned int w2) { edges[w1]->push_back(w2); }

    /**
     * @brief This method will parse the Kripke-CNF solution on the STDIN.
     *
     * @note the reading is mandatory in STDIN.
     */
    void parseSolutionFile();

    /** 
     * @brief This method is returning the assignation of the variable 'value' in the world 'world'
     * 
     * @param[in] world the world that we are checking.
     *
     * @param[in] value the ID of the atom from which we want the assignation.
     * 
     * @return the assignation of [world][value]
     *
     * @note it's in fact [world][value-1]. P0 doesn't exist, so P1 is stored at [world][0].
     */
    inline char getAssignedValue(unsigned int world,int value) { return assignations[world][value-1]; }

    /**
     * @brief This method is returning the list of worlds accessible from 'world'.
     * 
     * @param[in] world the ID of the world from which we want all the accessible one.
     * 
     * @return all the accessible worlds from 'world' in a vector.
     */
    vector<unsigned int>* getConnectedWorlds(unsigned int world) { return (this->edges[world]); }

    
    /**
     * @brief This method will return the number of connected worlds from 'world'.
     * 
     * @param[in] world the id of the world from which we want the number of connected one.
     *
     * @return the number of connected worlds from 'world'.
     */
    unsigned int getNbConnectedWorlds(unsigned int world) { return (this->edges[world])->size(); }

    /**
     * @brief This method will return the "next world" attached to world in parameter.
     *
     * @param[in] world the world from which we want the next one.
     * 
     * @note This method should be called only to get fastly a chain of world (when there is only 1 next world possible.)
     */
    unsigned int nextWorld(unsigned int world) { return (*this->edges[world])[0]; }

    /**
     * @brief This method will return the number of worlds in the current Kripke model.
     * 
     * @return the number of worlds in the current Kripke model.
     */
    unsigned int getNbWorlds() { return num_worlds; }

private:

	/** @brief How many propositional variables there are in the problem. */
    unsigned int num_variables;

    /** @brief How many worlds we use to generate the Kripke model. */
    unsigned int num_worlds;

    /** @brief What are the edges from world w_i to world w_j (store as a Matrix [nbWorlds][nbWorlds]). */
    vector<unsigned int>** edges;

    /** @brief What are the assignations of propositionals variables for each world (store as a Matrix [nbWorlds][nbVariables]). */
    vector<vector<int> > assignations;

};


#endif