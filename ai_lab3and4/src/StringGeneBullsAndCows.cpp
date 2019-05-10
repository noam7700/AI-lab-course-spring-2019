/*
 * StringGeneBullsAndCows.cpp
 *
 *  Created on: 13 ????? 2019
 *      Author: KY
 */

#include "StringGeneBullsAndCows.h"

StringGeneBullsAndCows::StringGeneBullsAndCows(float scaling_factor, int aging_factor):
    StringGene(scaling_factor, aging_factor) {
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
	    this->fitness = this->scaling_factor * fit + this->age;
}

void StringGeneBullsAndCows::setMate(Gene& p1, Gene& p2, Crossover_type xtype /*= CROSSOVER_DEFAULTX*/){
    switch(xtype){
        case CROSSOVER_DEFAULTX:
            this->setMateUniform(p1, p2);
            break;
        case CROSSOVER_UNIFORM:
            this->setMateUniform(p1, p2);
            break;
        case CROSSOVER_TWOPOINT:
            this->setMateTwoPoint(p1, p2);
            break;
        default:
            cout << "Error: No such Croosover_type\n";
            throw;
            break;
    }
    this->age = 0; //reset
}
void StringGeneBullsAndCows::setMateUniform(Gene& p1, Gene& p2){
    //for each offspring[i] choose randomly or from p1 or from p2

    StringGeneBullsAndCows& p1_casted = static_cast<StringGeneBullsAndCows&>(p1),
    p2_casted = static_cast<StringGeneBullsAndCows&>(p2);

    int r;
    for(unsigned int i=0; i<p2_casted.str.size(); i++){
        r = rand() % 2; //take from p1 or p2
        if(r == 0)
            this->str[i] = p1_casted.str[i];
        else
            this->str[i] = p2_casted.str[i];
    }
}
void StringGeneBullsAndCows::setMateTwoPoint(Gene& p1, Gene& p2){
    //this function assumes this->str is well defined - run_ga does init to buffer

    StringGeneBullsAndCows& p1_casted = static_cast<StringGeneBullsAndCows&>(p1),
    p2_casted = static_cast<StringGeneBullsAndCows&>(p2);

    int i = rand() % this->str.size(), j = rand() % this->str.size();
    if(i>j)
        swap(i,j); //I want i<j

    //3 sections [0,i) - p1's values, [i,j) - p2's values, [j,size()) - p1's values
    for(unsigned int index=0; index<i; index++){ //section 1
        this->str[index] = p1_casted.str[index];
    }
    for(unsigned int index=i; index<j; index++){ //section 2
        this->str[index] = p2_casted.str[index];
    }
    for(unsigned int index=j; index<this->str.size(); index++){ //section 3
        this->str[index] = p1_casted.str[index];
    }
}
