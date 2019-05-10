#include "Gene.h"

Gene::Gene(float fitness, float scaling_factor, int aging_factor){
    this->fitness = fitness;
    this->age = 0;
    this->scaling_factor = scaling_factor;
    this->aging_factor = aging_factor;
}

void Gene::copySetter(Gene& other){
    this->age = other.age + this->aging_factor;
}

float Gene::getFitness(){
    return this->fitness;
}
