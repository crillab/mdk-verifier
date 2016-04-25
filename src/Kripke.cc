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

/**
 * \file Kripke.cc
 * @brief Implementation of Kripke::methods which are not in the Kripke.h
 *
 * @author Valentin Montmirail
 * @version 1.0
 * @date 15/03/2016
 */

#include "../include/Kripke.h"

/** @brief We define the MAX width of a line in the Kripke-CNF file. It should normally never cross this limit. */
#define LIMIT_LINE 100000

/** 
 * @brief This method will assigned the atom 'value' in the world 'world'.
 * 
 * @param[in] value the atom that we want to assign (value < 0) if we want to atom negatively.
 *
 * @param[in] world the world in which the atom is assigned.
 */
void Kripke::assignValue(int value, unsigned int world) {

  unsigned int abs_value = abs(value);

  if(abs_value >= assignations[world].size()) {

    for(unsigned int i =0 ; i < num_worlds; i++) {
        assignations[i].resize(abs_value);
    }
  }

  /* If we want to assign p'value' positevely, we just put an 1 in [world][value-1] */
  if(value > 0)
	 assignations[world][value-1] = 1;

  /* If we want to assign p'value' negatively, we just put an -1 in [world][value-1] */
  else if(value < 0)
    assignations[world][abs_value-1] = -1;

}

/**
 * @brief This method will parse the Kripke-CNF solution on the STDIN.
 */
void Kripke::parseSolutionFile() {

  char str[LIMIT_LINE];
  FILE * file;
  int firstLine = 0;
  int descriptionSolution[4];
  int i = 0;
  int nbLines = 0;
  char* token = NULL;
  int w_begin = 0;
  int w_end = 0;;

  bool satFound = false;

  file = stdin;

  if (file) {

    while(fgets(str, LIMIT_LINE-1, file) != NULL) {
      
      if(str[0] == 'c') {
        continue;
      }

      if(strstr(str,"s UNSATISFIABLE")) {
        printf("ERROR: UNSATISFIABLE formulae are not checkable yet.\n\n");
        exit(0);
      }


      if(!satFound && strstr(str,"s SATISFIABLE")) {

        satFound = true;
      }

      if(str[0] == 'v') {
        
        if(firstLine == 0) {

          token = strtok(str+1, " ");
          while( token != NULL ) {

              descriptionSolution[i] = atoi(token);
              i++;
              
              /* Get next token: */
              token = strtok( NULL, " ");
          }

          num_variables = descriptionSolution[0];
          num_worlds    = descriptionSolution[1];

          if(num_variables == 0) {

              printf("UNKNOWN: No model was provided.\n\n");
              exit(0);
          }

            /* edges is a matrix [nbWorlds][nbWorlds] so we need to initialize it as an array of arrays */
            edges = (vector<unsigned>**)malloc(sizeof(vector<unsigned int>*)*num_worlds);
            assignations.resize(num_worlds);

            for(unsigned int i =0 ; i < num_worlds; i++) {
                edges[i] = new vector<unsigned>();
                assignations[i].resize(num_variables);
            }

        } else {

          if(nbLines < descriptionSolution[1]) {
            
            token = strtok(str+1, " ");
            i = 0;
            while( token != NULL ) {

              if(atoi(token) != 0) {

                int value = atoi(token);

                assignValue(value,nbLines);

                i++;
              }
              
              /* Get next token: */
              token = strtok( NULL, " ");
            }

          } else {

            token = strtok(str+1, " ");
            i = 0;

            while( token != NULL ) {

              if(i == 1) {
                w_begin = atoi(token+1); 
              }

              if(i == 2) {
                w_end = atoi(token+1); 
              }

              i++;

              /* Get next token: */
              token = strtok( NULL, " ");
            }

            addEdge(w_begin,w_end);
          }

          nbLines++;
        }

        firstLine = 1;
      }

    }

  } else {
    fprintf(stderr,"Problem with the solution file !\n");
    exit(-2);
  }

  if(!satFound) {
    printf("ERROR: solution not in Kripke-CNF.\n");
    exit(0);
  }
}