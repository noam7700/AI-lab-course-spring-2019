/*
 * Defs.h
 *
 *  Created on: 14 ????? 2019
 *      Author: KY
 */

#ifndef INCLUDE_DEFS_H_
#define INCLUDE_DEFS_H_

#include <iostream>					// for cout etc.
#include <vector>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <time.h>					// for random seed
#include <stdlib.h> //used for rand
#include <math.h>
#include <chrono>

#define GA_POPSIZE				2048		// ga population size
#define GA_MAXITER				16384		// maximum iterations
#define GA_ELITRATE				0.10f		// elitism rate
#define GA_MUTATIONRATE			0.25f		// mutation rate
#define GA_MUTATION				RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET				std::string("Hello world!")
#define GA_TOURNAMENT_SIZE 		16

typedef enum mate_type{
	MT_DEFAULT = 0,
	MT_TOURNAMENT = 1,
	MT_RWS = 2
} MateType;


#endif /* INCLUDE_DEFS_H_ */
