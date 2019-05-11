#include <iostream>
#include <math.h>
#include <stdlib.h> //used for rand
#include "StringGene.h"

string StringGene::target = "hello world";

StringGene::StringGene(float scaling_factor, int aging_factor, bool isUsingNiching, float sigma_share):
    Gene(0, scaling_factor, aging_factor, isUsingNiching, sigma_share){

}

void StringGene::print(){
    cout << this->str << "(" << this->fitness << ")" << endl;
}

void StringGene::init(){
    this->str.erase();
    for(unsigned int i=0; i<StringGene::target.size(); i++){
        this->str += (rand() % 90) + 32; //add random char. just like shay's code
    }
    this->age = 0; //even if we're not using aging, it still should be 0
}

void StringGene::calc_fitness(vector<Gene*>& gene_vector){
    int fit = 0;
    for(unsigned int i=0; i<StringGene::target.size(); i++){
        fit += abs(this->str[i] - StringGene::target[i]);
    }
    this->fitness = this->scaling_factor * fit + this->age; //will be 1 & 0 if not used
    this->fitness = this->sharing_fitness(this->fitness, gene_vector); //niching
}
void StringGene::mutate(Mutate_type mutype /*= MUTATE_DEFAULT*/){ //only one type, no need to check..
    int rand_index = rand() % StringGene::target.size();
    int delta = (rand() % 90) + 32;
    this->str[rand_index] = (this->str[rand_index] + delta) % 122;
}

void StringGene::setMate(Gene& p1, Gene& p2, Crossover_type xtype /*= CROSSOVER_DEFAULTX*/){ //only one type, no need to check..
    StringGene& p1_casted = static_cast<StringGene&>(p1), p2_casted = static_cast<StringGene&>(p2);
    int cut = rand() % StringGene::target.size();
    this->str = p1_casted.str.substr(0, cut) + p2_casted.str.substr(cut, StringGene::target.size() - cut);
    this->age = 0;
}

void StringGene::copySetter(Gene& other){ //we assume this & other are the same type
    Gene::copySetter(other); //will update age if needed
    StringGene& other_casted = static_cast<StringGene&>(other); //assuming other is StringGene
    this->str = other_casted.str;
    //no need to copy the fitness. calc_fitness will do it later
}

bool StringGene::isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer){
    return gene_vector[0]->getFitness() == 0;
}

bool StringGene::operator<(Gene& rh){
    return this->fitness < rh.getFitness();
}

float StringGene::dist(Gene& rh){
    StringGene& rh_casted = static_cast<StringGene&>(rh); //assuming rh is StringGene

    string& str1 = this->str;
    string& str2 = rh_casted.str;

    //not hamming distance! if the characters are not close, the distance should be bigger.
    //for example, <aaa,bbb> are more closer than <aaa,zzz>.
    float sum = 0;
    for(unsigned int i = 0; i < str1.size(); i++)
            sum += abs((int)str1[i] - (int)str2[i]);
    return sum;

}


