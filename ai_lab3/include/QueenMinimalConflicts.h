#ifndef QUEENMINIMALCONFLICTS_H
#define QUEENMINIMALCONFLICTS_H

#include <vector>
#include <algorithm>
using namespace std;

class QueenMinimalConflicts
{
    public:
        QueenMinimalConflicts(int num);
        void step(); //do the greedy step (minimum conflicts)

        virtual ~QueenMinimalConflicts() = default;

    protected:
        vector<int> queen_rows; //just like in QueenGene
};

#endif // QUEENMINIMALCONFLICTS_H
