#include "QueenMinimalConflicts.h"

QueenMinimalConflicts::QueenMinimalConflicts(int num)
{
    //just like init of QueenGene
    this->queen_rows = vector<int>(num);
    for(unsigned int i=0; i<queen_rows.size(); i++)
        queen_rows[i] = i; //identity permutation

    random_shuffle(queen_rows.begin(), queen_rows.end()); //shuffle the identity permutation, and get a random permutation
}

void QueenMinimalConflicts::print(){
    for(unsigned int i=0; i<queen_rows.size(); i++)
        cout << queen_rows[i] << " ";
    cout << "\n";
}

void QueenMinimalConflicts::solve(int MAX_IT){
    auto mini_start = chrono::high_resolution_clock::now();
    auto mini_finish = chrono::high_resolution_clock::now();
    auto start = chrono::high_resolution_clock::now();
    auto finish = chrono::high_resolution_clock::now();
    std::chrono::duration<double,milli> elapsed = mini_finish - mini_start;

    clock_t cmini_start = clock(), cmini_finish, cstart = clock(), cfinish;
    float clockticks;

    //code for checking if we reached to local minimum
    int rounds_till_failing = 100; //if we reach 100 rounds after last improvement we consider it as getting stuck
    float last_check_fit = (float)INT_MAX; //so in the first check we've definitely improved
    float current_check_fit;
    int didWeFail = 0, last_generation_improvement; //did we fail, and from which generation we got stuck

    //so we can show him how much time did it take to reach this local minimum
    auto last_improvement_mini_finish = chrono::high_resolution_clock::now();
    clock_t last_improvement_cmini_finish = clock();


    int i;
    for(int i=0; i<MAX_IT; i++){
        current_check_fit = this->step();
        if(current_check_fit == 0)
            break;
        cout << "(" << current_check_fit << ")\n";

        mini_finish = chrono::high_resolution_clock::now();
        elapsed = mini_finish - mini_start;
        cmini_finish = clock();
        clockticks = (float)(cmini_finish - cmini_start);

        cmini_start = cmini_finish; //switch between finish & start for next iteration
        mini_start = mini_finish; //same
        cout << "Step " << i << ": absolute time: " << elapsed.count() / 1000 << " clock ticks: " << clockticks << "\n";
        cout << "\n";

        //time for checking
        if(current_check_fit < last_check_fit){ //we imroved
            last_improvement_mini_finish = chrono::high_resolution_clock::now(); //so if we get stuck we can show real time
            last_improvement_cmini_finish = clock();
            last_check_fit = current_check_fit;
            rounds_till_failing = 100;
        }
        if(rounds_till_failing == 0){
            didWeFail = 1;
            last_generation_improvement = i - 100; //obviously..
            break;
        }

    }
    if(didWeFail == 1){
        finish = last_improvement_mini_finish;
        cfinish = last_improvement_cmini_finish;
    }
    else{
        finish = chrono::high_resolution_clock::now();
        cfinish = clock();
    }
    elapsed = finish - start;
    clockticks = cfinish - cstart;
    cout << "Overall: absolute time: " << elapsed.count() / 1000 << " clock ticks: " << clockticks << "\n";
    if(didWeFail == 1)
        cout << "We failed at generation " << last_generation_improvement << "\n";
    else
        cout << "We succeeded at generation " << i << "\n";


}

//if finished, return 0 and do nothing, else do step and return old "fitness"
int QueenMinimalConflicts::step(){
    //make taken_diagonals, like in QueenGene
    vector<int> taken_diagonals_desc(2 * queen_rows.size() - 1, 0); //init with 0. descending diagonals
    vector<int> taken_diagonals_asce(2 * queen_rows.size() - 1, 0); //init with 0. ascending diagonals

    //Descending: update taken_diagonals to number of queens in each diagonal
    int diagonal_index;
    for(unsigned int i=0; i<queen_rows.size(); i++){
        diagonal_index = queen_rows[i] - i;
        //cout << "Am i stupid? size=" << this->queen_rows.size() << endl;
        //cout << "I think I'm stupid yo-size=" << taken_diagonals_desc.size() << endl;
        //cout << "if left=" << diagonal_index << ". if up=" << -diagonal_index + this->queen_rows.size() << endl;
        if(diagonal_index < 0) //hits 'up' pixel instead of 'left' pixel
            taken_diagonals_desc[-diagonal_index + this->queen_rows.size() - 1]++;
        else
            taken_diagonals_desc[diagonal_index]++;
    }

    //Ascending: update taken_diagonals to number of queens in each diagonal
    for(unsigned int i=0; i<queen_rows.size(); i++){
        diagonal_index = queen_rows[i] + i; //now it doesn't matter if we step over N (hits 'down' pixel, and not 'left' pixel)
        taken_diagonals_asce[diagonal_index]++;
    }

    //now find best step - if needed
    //find worst queen (with most conflicts)
    int max_conflicts = INT_MIN, max_queen, curr_conflicts;
    int diagonal_index_desc, diagonal_index_asce;
    for(unsigned int i=0; i<queen_rows.size(); i++){
        diagonal_index_desc = queen_rows[i] - i;
        if(diagonal_index_desc < 0) //hits 'up' pixel instead of 'left' pixel
            diagonal_index_desc = -diagonal_index_desc + this->queen_rows.size() - 1;

        diagonal_index_asce = queen_rows[i] + i;

        curr_conflicts = taken_diagonals_asce[diagonal_index_asce] +
           taken_diagonals_desc[diagonal_index_desc] - 1 - 1;
        if(curr_conflicts > max_conflicts){
            max_conflicts = curr_conflicts;
            max_queen = i;
        }
    }

    //now find the best step for this worst queen
    int min_conflicts = INT_MAX, min_step;
    int curr_diagonal_index_desc, curr_diagonal_index_asce;
    for(int step=-queen_rows[max_queen]; step + queen_rows[max_queen] < queen_rows.size(); step++){
        //first calculate diagonals index after step
        curr_diagonal_index_desc = queen_rows[max_queen] + step - max_queen;
        if(curr_diagonal_index_desc < 0) //hits 'up' pixel instead of 'left' pixel
            curr_diagonal_index_desc = -curr_diagonal_index_desc + this->queen_rows.size() - 1;

        curr_diagonal_index_asce = queen_rows[max_queen] + step + max_queen;

        //calculate conflicts after step
        curr_conflicts = taken_diagonals_asce[curr_diagonal_index_asce] - 1;
        curr_conflicts += taken_diagonals_desc[curr_diagonal_index_desc] - 1;

        if(curr_conflicts < min_conflicts){
            min_conflicts = curr_conflicts;
            min_step = step;
        }
    }

    if(max_conflicts == 0){ //finished!
        return 0; //do nothing!
    }
    else{
        queen_rows[max_queen] += min_step; //do the step

        //calculate old fitness
        int old_fitness = 0;
        for(unsigned int i=0; i<taken_diagonals_asce.size(); i++)
            old_fitness += taken_diagonals_asce[i] == 0 ? 0 : taken_diagonals_asce[i] - 1;
        for(unsigned int i=0; i<taken_diagonals_desc.size(); i++)
            old_fitness += taken_diagonals_desc[i] == 0 ? 0 : taken_diagonals_desc[i] - 1;

        return old_fitness;
    }
}

