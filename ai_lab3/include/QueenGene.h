#ifndef QUEENGENE_H
#define QUEENGENE_H

#include <Gene.h>


class QueenGene : public Gene
{
    public:
        QueenGene(int num);

        bool operator<(Gene& rh);

        void init();
        void calc_fitness();
        void mutate();
        void setMate(Gene& p1, Gene& p2);
        void copySetter(Gene& other);
        bool isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer);
        void print();


        virtual ~QueenGene() = default;

    protected:
        vector<int> queen_rows;
        vector<int> taken_diagonal_line;
};

#endif // QUEENGENE_H
