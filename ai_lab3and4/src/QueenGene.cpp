#include "QueenGene.h"

vector<int> QueenGene::taken_diagonals;

QueenGene::QueenGene(float scaling_factor, int aging_factor, int num, bool isUsingNiching, float sigma_share):
    Gene::Gene(0, scaling_factor, aging_factor, isUsingNiching, sigma_share){
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
    this->age = 0; //even if we're not using aging, it still should be 0
}

void QueenGene::calc_fitness(vector<Gene*>& gene_vector){
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
    this->fitness = this->scaling_factor * fit + this->age; //even if it's not in use
    this->fitness = this->sharing_fitness(this->fitness, gene_vector); //niching

}

void QueenGene::mutate(Mutate_type mutype /*= MUTATE_DEFAULT*/){
    switch(mutype){
        case MUTATE_DEFAULT:
            //mutateSwap is the best!
            //mutateSwap can never get stuck, because there's always a possible mutation which can improve
            this->mutateSwap();
            break;
        case MUTATE_SWAP:
            this->mutateSwap();
            break;
        case MUTATE_SIM:
            //can sometimes get stuck in local max
            this->mutateSim();
            break;
        default:
            cout << "Error: No such Mutate option\n";
            throw;
            break;

    }
}

void QueenGene::mutateSwap(){
    //mutate the vector, but keep it a permutation. just swap between two random indexes
    int i = rand() % this->queen_rows.size();
    int delta = rand() % (this->queen_rows.size() - 1) + 1;
    int j = (i + delta) % this->queen_rows.size(); //that way i != j
    swap(this->queen_rows[i], this->queen_rows[j]);
}

void QueenGene::mutateSim(){
    //mutate the vector, but keep it a permutation. simple inversion mutation
    int window_size = 5;
    int start_index = rand() % this->queen_rows.size();
    int end_index = start_index + (window_size - 1);
    end_index = min(end_index, (int)(this->queen_rows.size() - 1)); //dont exceed vector size

    for(int i = start_index, j = end_index; i<j; i++, j--)
        swap(this->queen_rows[i], this->queen_rows[j]);
}

void QueenGene::setMate(Gene& p1, Gene& p2, Crossover_type xtype /*= CROSSOVER_DEFAULTX*/){
    switch(xtype){
        case CROSSOVER_DEFAULTX:
            this->setMateCX(p1, p2);
            break;
        case CROSSOVER_CX:
            this->setMateCX(p1, p2);
            break;
        case CROSSOVER_OX:
            this->setMateOX(p1, p2);
            break;
        default:
            cout << "Error: No such Crossover_type\n";
            throw;
            break;
    }

    this->age = 0;
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

    //they're const, I'm not gonna destroy them. dont worry :)
    const vector<int>& perm1 = p1_casted.queen_rows, perm2 = p2_casted.queen_rows;

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

void QueenGene::setMateOX(Gene& p1, Gene& p2){
    //pick half values from p1, and the other half from p2 with the same order
    /*
    step 1 - copy perm1 to vector<pair<int,int>> so it has perm1 & his original indices
    step 2 - scramble perm1's copy
    step 2.5 - create counting array 0 to N-1 to which value has been added to offspring (used for step 4)
    also, reset offspring array to -1, so we know which indices were already touched
    step 3 - take the first half of the copied array (random sub-group of n/2 size).
    and put them in original indices in offspring gene (while updating array of 2.5)
    step 4 - put in the remaining values of perm2's (the values that are 0 in counting array)
    *put them in the same order they're at perm2

    */
    QueenGene& p1_casted = static_cast<QueenGene&>(p1);
    QueenGene& p2_casted = static_cast<QueenGene&>(p2);

    //they're const, I'm not gonna destroy them. dont worry :)
    const vector<int>& perm1 = p1_casted.queen_rows, perm2 = p2_casted.queen_rows;

    //step 1
    //for each i: (perm1[i], i). copy of perm1, but save his index
    vector<pair<int,int>> perm1_scrambled(perm1.size());
    for(unsigned int i=0; i<perm1.size(); i++){
        perm1_scrambled[i].first = perm1[i];
        perm1_scrambled[i].second = i;
    }

    //step 2
    //when comparing pair<int,int> it compares the .first & only if they're equal it compares .second
    random_shuffle(perm1_scrambled.begin(), perm1_scrambled.end());

    //step 2.5
    vector<int> touched_values(perm1.size(), 0); //init as 0
    for(unsigned int i=0; i<this->queen_rows.size(); i++)
        this->queen_rows[i] = -1; //mark as untouched

    //step 3
    for(unsigned int i=0; i < (perm1_scrambled.size() / 2); i++){
        this->queen_rows[perm1_scrambled[i].second] = perm1_scrambled[i].first;
        touched_values[perm1_scrambled[i].first]++; //counting array
    }

    //step 4
    int offspring_index = 0;
    for(unsigned int i=0; i<perm2.size(); i++) //needs to go through the order of perm2
        if(touched_values[perm2[i]] == 0){ //untouched
            //we found value from perm2 that needs to be put in offspring
            while(offspring_index < this->queen_rows.size()){ //unused condition, but screw it
                if(this->queen_rows[offspring_index] == -1){ //free slot
                    this->queen_rows[offspring_index] = perm2[i];
                    offspring_index++;
                    break;
                }
                offspring_index++;
            }
            touched_values[perm2[i]]++; //I dont think it's necessary, but screw it
        }
    return;
}

void QueenGene::copySetter(Gene& other){
    Gene::copySetter(other); //update age if needs to
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

float QueenGene::dist(Gene& rh){
    QueenGene& rh_casted = static_cast<QueenGene&>(rh); //assuming rh is QueenGene

    vector<int>& queen_rows1 = this->queen_rows;
    vector<int>& queen_rows2 = rh_casted.queen_rows;

    //hamming distance
    float sum = 0;
    for(unsigned int i = 0; i < queen_rows1.size(); i++)
        if(queen_rows1[i] != queen_rows2[i])
            sum += 1.0f;
    return sum;
}



