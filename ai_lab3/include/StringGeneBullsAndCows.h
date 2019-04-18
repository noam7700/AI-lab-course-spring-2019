/*
 * StringGeneBullsAndCows.h
 *
 *  Created on: 13 ????? 2019
 *      Author: KY
 */

#ifndef INCLUDE_STRINGGENEBULLSANDCOWS_H_
#define INCLUDE_STRINGGENEBULLSANDCOWS_H_

#include <iostream>
#include <StringGene.h>

class StringGeneBullsAndCows: public StringGene {
public:
	StringGeneBullsAndCows();
    void calc_fitness(); //update calc_fitness
    void setMate(Gene& p1, Gene& p2, Crossover_type xtype = CROSSOVER_DEFAULTX); //set attributes as the son of p1 & p2
    void setMateUniform(Gene& p1, Gene& p2); //option to setMate - "uniform crossover"
    void setMateTwoPoint(Gene& p1, Gene& p2); //option to setMate - "two point crossover"
	virtual ~StringGeneBullsAndCows() = default;
};

#endif /* INCLUDE_STRINGGENEBULLSANDCOWS_H_ */
