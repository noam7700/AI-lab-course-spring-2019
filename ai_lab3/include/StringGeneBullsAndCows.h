/*
 * StringGeneBullsAndCows.h
 *
 *  Created on: 13 ????? 2019
 *      Author: KY
 */

#ifndef INCLUDE_STRINGGENEBULLSANDCOWS_H_
#define INCLUDE_STRINGGENEBULLSANDCOWS_H_

#include <StringGene.h>

class StringGeneBullsAndCows: public StringGene {
public:
	StringGeneBullsAndCows();
    void calc_fitness(); //update calc_fitness

	virtual ~StringGeneBullsAndCows() = default;
};

#endif /* INCLUDE_STRINGGENEBULLSANDCOWS_H_ */
