#include "StringGeneScaling.h"
#include <iostream>

using namespace std;

StringGeneScaling::StringGeneScaling()
{
    //ctor
}

void StringGeneScaling::calc_fitness(){
    StringGene::calc_fitness();
    float regular_fit = this->fitness;
    float factor = 0.2f; //to depower "super heros"
    this->fitness = factor * regular_fit;
}
