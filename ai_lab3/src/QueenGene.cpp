#include "QueenGene.h"

vector<int> QueenGene::taken_diagonals;

QueenGene::QueenGene(int num): Gene::Gene(0){
    this->queen_rows = vector<int>(num);

    //only allocate his memory once. doesn't work in multi-thread
    static int first_time_called = 1;
    if(first_time_called == 1){
        //number of diagonals of N by N is: #left border pixels + #top border pixels - 1 (top-left corner counted twice)
        QueenGene::taken_diagonals = vector<int>(2 * num - 1);
        first_time_called = 0;
    }
}

bool QueenGene::operator<(Gene& rh){
    return this->fitness < rh.getFitness(); //the smaller the better. "good things come in small packages" :P
}

void QueenGene::init(){

    for(unsigned int i=0; i<queen_rows.size(); i++)
        queen_rows[i] = i; //identity permutation

    random_shuffle(queen_rows.begin(), queen_rows.end()); //shuffle the identity permutation, and get a random permutation
}

void QueenGene::calc_fitness(){
    float fit = 0;

    //Descending: update taken_diagonals to number of queens in each diagonal
    //first clean taken_diagonals
    for(unsigned int i=0; i<QueenGene::taken_diagonals.size(); i++)
        QueenGene::taken_diagonals[i] = 0;

    int diagonal_index;
    for(unsigned int i=0; i<queen_rows.size(); i++){
        diagonal_index = queen_rows[i] - i;
        if(diagonal_index < 0) //hits 'up' pixel instead of 'left' pixel
            QueenGene::taken_diagonals[-diagonal_index + this->queen_rows.size()]++;
        else
            QueenGene::taken_diagonals[diagonal_index]++;
    }
    for(unsigned int i=0; i<QueenGene::taken_diagonals.size(); i++)
        //number of pairs in this diagonal is k*(k-1)/2
        fit += QueenGene::taken_diagonals[i] * (QueenGene::taken_diagonals[i] - 1) / 2.0f;

    //Ascending: update taken_diagonals to number of queens in each diagonal
    //first clean taken_diagonals
    for(unsigned int i=0; i<QueenGene::taken_diagonals.size(); i++)
        QueenGene::taken_diagonals[i] = 0;

    for(unsigned int i=0; i<queen_rows.size(); i++){
        diagonal_index = queen_rows[i] + i; //now it doesn't matter if we step over N (hits 'down' pixel, and not 'left' pixel)
        QueenGene::taken_diagonals[diagonal_index]++;
    }

    for(unsigned int i=0; i<QueenGene::taken_diagonals.size(); i++)
        //number of pairs in this diagonal is k*(k-1)/2
        fit += QueenGene::taken_diagonals[i] * (QueenGene::taken_diagonals[i] - 1) / 2.0f;

    //dont forget to set the attribute itself
    this->fitness = fit;

}

void QueenGene::mutate(Mutate_type mutype /*= MUTATE_DEFAULT*/){
    this->mutateSwap();
}

void QueenGene::mutateSwap(){
    //mutate the vector, but keep it a permutation. just swap between two random indexes
    int i = rand() % this->queen_rows.size();
    int delta = rand() % (this->queen_rows.size() - 1) + 1;
    int j = (i + delta) % this->queen_rows.size(); //that way i != j
    swap(this->queen_rows[i], this->queen_rows[j]);
}

void QueenGene::setMate(Gene& p1, Gene& p2, Crossover_type xtype /*= CROSSOVER_DEFAULTX*/){
    this->setMateCX(p1, p2);
}

void QueenGene::setMateCX(Gene& p1, Gene& p2){
    //complexity - O(N) (N is #Queens)
    /*
    step 1 - create inverse permutation for p1 & p2
    step 2 - find "combined cycles"
    step 3 - set the offspring (this object) with alternating cycles of p1 & p2
    */
    QueenGene& p1_casted = static_cast<QueenGene&>(p1);
    QueenGene& p2_casted = static_cast<QueenGene&>(p2);

    const vector<int>& perm1 = p1_casted.queen_rows, perm2 = p2_casted.queen_rows; //they're const, I'm not gonna destroy them

    //step 1:
    vector<int> rev_perm1(perm1.size()), rev_perm2(perm2.size());
    for(unsigned int i=0; i<perm1.size(); i++)
        rev_perm1[perm1[i]] = i; //perm1[i] is in index i (reverse permutation)
    for(unsigned int i=0; i<perm2.size(); i++)
        rev_perm2[perm2[i]] = i; //perm2[i] is in index i (reverse permutation)

    //step 2:
    //TODO: maybe later on we will use QueenGene::taken_diagonals to save memory allocations (doesn't seem too important)
    vector<int> taken_cells(perm1.size(), -1); //initialize it with -1

    int starting_index = 0, curr_index = 0;
    int current_cycle = 0; // used for taken_cells, counting starts at 0

    while(starting_index < perm1.size()){
        curr_index = starting_index;
        while(!(curr_index == starting_index && taken_cells[starting_index] != -1)){ //we didn't reach the start the second time
            taken_cells[curr_index] = current_cycle; //mark him
            //next index:
            curr_index = rev_perm1[perm2[curr_index]]; //find the index in which perm2[curr_index] shows in perm1
        }

        //get ready for next cycle
        current_cycle++;
        while(taken_cells[starting_index] != -1 && starting_index < perm1.size()) //find next (unmarked) cycle starting index
            starting_index++;
    }

    //step 3
    for(unsigned int i=0; i<this->queen_rows.size(); i++){
        if(taken_cells[i] % 2 == 0) //even cycle
            this->queen_rows[i] = perm1[i];
        else
            this->queen_rows[i] = perm2[i];
    }
}

void QueenGene::copySetter(Gene& other){
    QueenGene& other_casted = static_cast<QueenGene&>(other); //assuming other is QueenGene
    //deep copy w/o allocations (I don't trust operator= of vectors because they might re-allocate the memory)
    for(unsigned int i=0; i<this->queen_rows.size(); i++)
        this->queen_rows[i] = other_casted.queen_rows[i];
}

bool QueenGene::isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer){
    if(gene_vector[0]->getFitness() == 0)
        return true;
    return false;
}

void QueenGene::print(){
    /*for(unsigned int i=0; i<this->queen_rows.size(); i++)
        cout << this->queen_rows[i];*/ //too long didn't read :P - where #Queens is 1000...
    cout << "(" << this->fitness << ")\n";
}




