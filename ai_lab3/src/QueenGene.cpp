#include "QueenGene.h"
#include <stdlib.h> //used for rand

QueenGene::QueenGene(int num): Gene::Gene(0){
    this->queen_rows = vector<int>(num);
    //number of diagonals for each starting point on the top/left. the top-left corner is common to both top & left
    this->taken_diagonal_line = vector<int>(2 * num - 1);
}

bool QueenGene::operator<(Gene& rh){
    return this->fitness < rh.getFitness();
}

//this gives some random vector!! we need random permutation!@$%@ (that's what shay recomended)
void QueenGene::init(){

    int diagonal_index;
    for(unsigned int i=0; i<queen_rows.size(); i++){
        queen_rows[i] = rand() % queen_rows.size();

        //update current diagonal to be taken
        diagonal_index = queen_rows[i] - i;
        if(diagonal_index < 0){ //it's starting on some upper pixel (at the top, not at the left)

        }
        taken_diagonal_line[-diagonal_index + queen_rows.size()] = 1;
    }
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




