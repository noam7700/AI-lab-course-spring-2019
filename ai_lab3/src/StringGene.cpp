#include <iostream>
#include <math.h>
#include <stdlib.h> //used for rand
#include "StringGene.h"

string StringGene::target = "hello world";

StringGene::StringGene(): Gene(0){

}

void StringGene::print(){
    cout << this->str << "(" << this->fitness << ")" << endl;
}

void StringGene::init(){
    this->str.erase();
    for(unsigned int i=0; i<StringGene::target.size(); i++){
        this->str += (rand() % 90) + 32; //add random char. just like shay's code
    }
}

void StringGene::calc_fitness(){
    int fit = 0;
    for(unsigned int i=0; i<StringGene::target.size(); i++){
        fit += abs(this->str[i] - StringGene::target[i]);
    }
    this->fitness = fit;
}
void StringGene::mutate(){
    int rand_index = rand() % StringGene::target.size();
    int delta = (rand() % 90) + 32;
    this->str[rand_index] = (this->str[rand_index] + delta) % 122;
}

void StringGene::setMate(Gene& p1, Gene& p2){
     StringGene& p1_casted = static_cast<StringGene&>(p1), p2_casted = static_cast<StringGene&>(p2);
    int cut = rand() % StringGene::target.size();
    this->str = p1_casted.str.substr(0, cut) + p2_casted.str.substr(cut, StringGene::target.size() - cut);
}

void StringGene::copySetter(Gene& other){ //we assume this & other are the same type
    StringGene& other_casted = static_cast<StringGene&>(other); //assuming other is StringGene
    this->str = other_casted.str;
    this->fitness = other_casted.fitness;
}

bool StringGene::isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer){
    return gene_vector[0]->getFitness() == 0;
}

bool StringGene::operator<(Gene& rh){
    return this->fitness < rh.getFitness();
}


