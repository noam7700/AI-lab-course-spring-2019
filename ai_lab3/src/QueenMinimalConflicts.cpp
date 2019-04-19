#include "QueenMinimalConflicts.h"

QueenMinimalConflicts::QueenMinimalConflicts(int num)
{
    //just like init of QueenGene
    this->queen_rows = vector<int>(num);
    for(unsigned int i=0; i<queen_rows.size(); i++)
        queen_rows[i] = i; //identity permutation

    random_shuffle(queen_rows.begin(), queen_rows.end()); //shuffle the identity permutation, and get a random permutation
}

//if finished, return true and do nothing. else, do best step, return false
bool QueenMinimalConflicts::step(){
    //make taken_diagonals, like in QueenGene
    vector<int> taken_diagonals_desc(2 * queen_rows.size() - 1, 0); //init with 0. descending diagonals
    vector<int> taken_diagonals_asce(2 * queen_rows.size() - 1, 0); //init with 0. ascending diagonals

    //Descending: update taken_diagonals to number of queens in each diagonal
    int diagonal_index;
    for(unsigned int i=0; i<queen_rows.size(); i++){
        diagonal_index = queen_rows[i] - i;
        if(diagonal_index < 0) //hits 'up' pixel instead of 'left' pixel
            taken_diagonals_desc[-diagonal_index + this->queen_rows.size()]++;
        else
            taken_diagonals_desc[diagonal_index]++;

    //Ascending: update taken_diagonals to number of queens in each diagonal
    for(unsigned int i=0; i<queen_rows.size(); i++){
        diagonal_index = queen_rows[i] + i; //now it doesn't matter if we step over N (hits 'down' pixel, and not 'left' pixel)
        taken_diagonals_asce[diagonal_index]++;
    }

    //now find best step - if needed
    int diagonal_index_desc, diagonal_index_asce;
    int number_of_nice_queens = 0;
    int min_conflicts_queen;
    int min_conflicts_step; //delta of the queen where to go (go -2 steps or +3 steps and such..)
    int min_value = INT_MAX;
    int curr_conflicts_value;
    int curr_diagonal_index_desc, curr_diagonal_index_asce;
    for(unsigned int i=0; i<queen_rows.size(); i++){
        diagonal_index_desc = queen_rows[i] - i;
        if(diagonal_index < 0) //hits 'up' pixel instead of 'left' pixel
            diagonal_index_desc = -diagonal_index_desc + this->queen_rows.size();

        diagonal_index_asce = queen_rows[i] + i;

        if(taken_diagonals_asce[diagonal_index_diagonal_index_asce] == 1 &&
           taken_diagonals_desc[diagonal_index_desc] == 1){ //no conflicts in both diagonals
                number_of_nice_queens++;
           }

        //check all steps. if queen in index p, all legal steps are in range of [-p,N-p)
        for(int step=-queen_rows[i]; step + queen_rows[i] < queen_rows.size(); steps++){
            //first calculate diagonals index after step
            curr_diagonal_index_desc = queen_rows[i] + steps - i;
            if(curr_diagonal_index_desc < 0) //hits 'up' pixel instead of 'left' pixel
            curr_diagonal_index_desc = -curr_diagonal_index_desc + this->queen_rows.size();

            curr_diagonal_index_asce = queen_rows[i] + steps + i;

            curr_conflicts_value = taken_diagonals_asce[curr_diagonal_index_asce] - 1; //#others Qs in his asce' dia'
            curr_conflicts_value += taken_diagonals_desc[curr_diagonal_index_desc] - 1; //#other Qs in his desc' dia'
            if(curr_conflicts_step < min_value){
                min_value = curr_conflicts_value;
                min_conflicts_step = step;
                min_conflicts_queen = i;
            }
        } //inner step-loop
    } //outer queen-loop

    if(number_of_nice_queens == queen_rows.size()){ //finished!
        return true; //do nothing!
    }
    else{
        queen_rows[min_conflicts_queen] += min_conflicts_step; //do the step
        return false;
    }
}
