# ***************************************************************************************
# Copyright (c) 2016 Artois University
#
# This file is part of MDK-Verifier
#
# MDK-Verifier is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free Software Foundation, 
# either version 3 of the License, or (at your option) any later version.
#
# MDK-Verifier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with MDK-Verifier. 
# If not, see http://www.gnu.org/licenses/.
#
# Contributors:
#		- Valentin Montmirail (valentin.montmirail@cril.fr)
#**************************************************************************************************/

CXX       	= g++
CC       	= gcc

BISON       = bison
BISON_OPT   = -d -t -l -v -o

FLEX        = flex
FLEX_OPT    = -l -b
	
CFLAGS   	= -fPIC -std=c++11 
POST_CFLAGS = 
LFLAGS    	= 
OPT 		= -O3 -g -pg

BIN			= ./bin
OBJ 	    = ./obj
SRC 	    = ./src
GRAMM 		= ./grammar
INCLUDE 	= ./include

EXEC 	    = $(BIN)/mdk-verifier

ALL_OBJECTS = $(OBJ)/modal.tab.o 				  \
			  $(OBJ)/lex.yy.o   			  	  \
			  $(OBJ)/Kripke.o    				  \
			  $(OBJ)/Term.o      				  \
			  $(OBJ)/BooleanConstant.o      	  \
			  $(OBJ)/PropositionalVariable.o      \
			  $(OBJ)/Variable.o      			  \
			  $(OBJ)/Operation.o      			  \
			  $(OBJ)/BooleanOperation.o     	  \
			  $(OBJ)/ModalOperation.o     	      \
			  $(OBJ)/Formula.o     	 			      	            

SRC_MAIN    = $(SRC)/Main.cc

SRC_OPERATION    		 = $(SRC)/Operation.cc
SRC_BOOLEAN_OPERATION    = $(SRC)/BooleanOperation.cc
SRC_MODAL_OPERATION      = $(SRC)/ModalOperation.cc

SRC_FORMULA    			 = $(SRC)/Formula.cc
SRC_VARIABLE    	 	 = $(SRC)/Variable.cc
SRC_PROP_VARIABLE    	 = $(SRC)/PropositionalVariable.cc
SRC_BOOLEAN_CONSTANT 	 = $(SRC)/BooleanConstant.cc
SRC_TERM    		 	 = $(SRC)/Term.cc

SRC_KRIPKE				 = $(SRC)/Kripke.cc

##############################################################################################################

all: $(EXEC)

##############################################################################################################

$(EXEC): $(ALL_OBJECTS) $(OBJ)/Main.o
	$(CXX) $(OPT) $(LFLAGS) -o $(EXEC) $(ALL_OBJECTS) $(OBJ)/Main.o

##############################################################################################################

$(GRAMM)/modal.tab.h: $(GRAMM)/modal.y 
	$(BISON) $(BISON_OPT) $(GRAMM)/modal.tab.c $(GRAMM)/modal.y && mv $(GRAMM)/modal.tab.c $(GRAMM)/modal.tab.cpp

$(GRAMM)/modal.tab.cpp: $(GRAMM)/modal.y 
	$(BISON) $(BISON_OPT) $(GRAMM)/modal.tab.c $(GRAMM)/modal.y && mv $(GRAMM)/modal.tab.c $(GRAMM)/modal.tab.cpp

$(OBJ)/modal.tab.o: $(GRAMM)/modal.tab.cpp $(GRAMM)/modal.tab.h
	$(CXX) $(OPT) $(CFLAGS) -I$(INCLUDE) -c $(GRAMM)/modal.tab.cpp -o $(OBJ)/modal.tab.o

##############################################################################################################

$(GRAMM)/lex.yy.cpp: $(GRAMM)/modal.tab.h $(GRAMM)/modal.lex 
	$(FLEX) $(FLEX_OPT) -o$(GRAMM)/lex.yy.cpp $(GRAMM)/modal.lex 	

$(OBJ)/lex.yy.o: $(GRAMM)/lex.yy.cpp 
	$(CXX) $(OPT) -I$(INCLUDE) -c $(GRAMM)/lex.yy.cpp -o $(OBJ)/lex.yy.o

##############################################################################################################

$(OBJ)/Term.o: $(SRC_TERM) $(INCLUDE)/Term.h $(OBJ)/modal.tab.o
	$(CXX) $(OPT) $(CFLAGS) -I$(INCLUDE) -c $(SRC_TERM) -o $(OBJ)/Term.o $(POST_CFLAGS)

$(OBJ)/BooleanConstant.o: $(SRC_BOOLEAN_CONSTANT) $(INCLUDE)/BooleanConstant.h $(OBJ)/Term.o
	$(CXX) $(OPT) $(CFLAGS) -I$(INCLUDE) -c $(SRC_BOOLEAN_CONSTANT) -o $(OBJ)/BooleanConstant.o $(POST_CFLAGS)

$(OBJ)/PropositionalVariable.o: $(SRC_PROP_VARIABLE) $(INCLUDE)/PropositionalVariable.h $(OBJ)/Variable.o
	$(CXX) $(OPT) $(CFLAGS) -I$(INCLUDE) -c $(SRC_PROP_VARIABLE) -o $(OBJ)/PropositionalVariable.o $(POST_CFLAGS)

$(OBJ)/Variable.o: $(SRC_VARIABLE) $(INCLUDE)/Variable.h $(OBJ)/Term.o
	$(CXX) $(OPT) $(CFLAGS) -I$(INCLUDE) -c $(SRC_VARIABLE) -o $(OBJ)/Variable.o $(POST_CFLAGS)

$(OBJ)/Operation.o: $(SRC_OPERATION) $(INCLUDE)/Operation.h $(OBJ)/Term.o
	$(CXX) $(OPT) $(CFLAGS) -I$(INCLUDE) -c $(SRC_OPERATION) -o $(OBJ)/Operation.o $(POST_CFLAGS)

$(OBJ)/BooleanOperation.o: $(SRC_BOOLEAN_OPERATION) $(INCLUDE)/BooleanOperation.h $(OBJ)/Operation.o
	$(CXX) $(OPT) $(CFLAGS) -I$(INCLUDE) -c $(SRC_BOOLEAN_OPERATION) -o $(OBJ)/BooleanOperation.o $(POST_CFLAGS)

$(OBJ)/ModalOperation.o: $(SRC_MODAL_OPERATION) $(INCLUDE)/ModalOperation.h $(OBJ)/Operation.o
	$(CXX) $(OPT) $(CFLAGS) -I$(INCLUDE) -c $(SRC_MODAL_OPERATION) -o $(OBJ)/ModalOperation.o $(POST_CFLAGS)	

$(OBJ)/Formula.o: $(SRC_FORMULA) $(INCLUDE)/Formula.h $(INCLUDE)/Term.h 
	$(CXX) $(OPT) $(CFLAGS) -I$(INCLUDE) -c $(SRC_FORMULA) -o $(OBJ)/Formula.o $(POST_CFLAGS)

$(OBJ)/Kripke.o: $(SRC_KRIPKE)
	$(CXX) $(OPT) $(CFLAGS) -I$(INCLUDE) -c $(SRC_KRIPKE) -o $(OBJ)/Kripke.o $(POST_CFLAGS)	

$(OBJ)/Main.o: $(SRC_MAIN)
	$(CXX) $(OPT) $(CFLAGS) -o $(OBJ)/Main.o -c $(SRC_MAIN) $(POST_CFLAGS)

$(OBJ)/Display.o: $(SRC)/Display.cc
	$(CXX) $(OPT) $(CFLAGS) -o $(OBJ)/Display.o -c $(SRC)/Display.cc $(POST_CFLAGS)	

##############################################################################################################	

clean:
	rm -f $(OBJ)/*.o
	rm -f $(EXEC)
	rm -f $(GRAMM)/modal.output
	rm -f $(GRAMM)/modal.tab.h
	rm -f $(GRAMM)/*.cpp
	rm -f ./lex.backup

##############################################################################################################	
