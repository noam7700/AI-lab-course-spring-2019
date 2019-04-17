#include "StringGeneAging.h"

StringGeneAging::StringGeneAging()
{
    //ctor
}

void StringGeneAging::calc_fitness(){
    StringGene::calc_fitness();
    float regular_fit = this->fitness;
    this->fitness = regular_fit + this->age / 4;
}

void StringGeneAging::init(){
    StringGene::init();
    this->age = 0;
}

void StringGeneAging::copySetter(Gene& other){
    StringGeneAging& other_casted = static_cast<StringGeneAging&>(other); //assuming other is StringGene
    this->str = other_casted.str;
    this->age = other_casted.age + 1;
}

void StringGeneAging::setMate(Gene& p1, Gene& p2, Crossover_type xtype /*= CROSSOVER_DEFAULTX*/){ //not gonna use xtype
    StringGene::setMate(p1, p2);
    this->age = 0; //son born. he's 0 years old
}



