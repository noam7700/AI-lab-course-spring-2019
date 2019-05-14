#ifndef QUEENMINIMALCONFLICTS_H
#define QUEENMINIMALCONFLICTS_H

#include "Defs.h"

using namespace std;

class QueenMinimalConflicts
{
    public:
        QueenMinimalConflicts(int num);
        void solve(int MAX_IT); //do loop of steps
        int step(); //do the greedy step (minimum conflicts)
        void print();

        virtual ~QueenMinimalConflicts() = default;

    //protected:
        vector<int> queen_rows; //just like in QueenGene
};

#endif // QUEENMINIMALCONFLICTS_H
