#include "Gene.h"

Gene::Gene(float fitness){
    this->fitness = fitness;
}

float Gene::getFitness(){
    return this->fitness;
}
