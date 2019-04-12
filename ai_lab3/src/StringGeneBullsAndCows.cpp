/*
 * StringGeneBullsAndCows.cpp
 *
 *  Created on: 13 ????? 2019
 *      Author: KY
 */

#include "StringGeneBullsAndCows.h"

StringGeneBullsAndCows::StringGeneBullsAndCows():StringGene() {
	// TODO Auto-generated constructor stub

}

void StringGeneBullsAndCows::calc_fitness() {
	    int fit = 0;
	    for(unsigned int i=0; i<StringGene::target.size(); i++){
	        if(this->str[i] != StringGene::target[i])
	        {
	        	fit += 2;
	        }
	        if(StringGene::target.find(this->str[i]) == std::string::npos)
	        {
	        	fit += 1;
	        }
	    }
	    this->fitness = fit;
}
