#include "QueenGene.h"

QueenGene::QueenGene(int num): Gene::Gene(0){
    this->queen_rows = vector<int>(num);
}

bool QueenGene::operator<(Gene& rh){
    return this->fitness < rh.getFitness();
}

//this gives some random vector!! we need random permutation!@$%@ (that's what shay recomended)
void QueenGene::init(){

    for(unsigned int i=0; i<queen_rows.size(); i++)
        queen_rows[i] = i; //identity permutation

    random_shuffle(queen_rows.begin(), queen_rows.end()); //shuffle the identity permutation, and get a random permutation
}

void QueenGene::calc_fitness(){
    //TODO:
}
void QueenGene::mutate(){

}
void QueenGene::setMate(Gene& p1, Gene& p2){

}
void QueenGene::copySetter(Gene& other){

}
bool QueenGene::isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer){
    return 1;
}
void QueenGene::print(){

}




