#ifndef QUEENGENE_H
#define QUEENGENE_H

#include <iostream>
#include <Gene.h>
#include <stdlib.h> //used for rand
#include <algorithm> //for random_shuffle

class QueenGene : public Gene
{
    public:
        QueenGene(int num);

        /*
        Each time we want to calculate diagonal intersections, we need this kind of array (of size 2*num - 1).
        therefor, create this array once, and re-assign it over & over to save memory allocations.
        btw, it'll be used for both descending and ascending diagonals
        */
        static vector<int> taken_diagonals;

        bool operator<(Gene& rh);

        void init();
        void calc_fitness();
        void mutate(Mutate_type mutype = MUTATE_DEFAULT);
        void mutateSwap(); //option to mutate - "swap mutation"
        void mutateSim(); //option to mutate - "simple inversion mutation"
        void setMate(Gene& p1, Gene& p2, Crossover_type xtype = CROSSOVER_DEFAULTX); //set attributes as the son of p1 & p2
        void setMateOX(Gene& p1, Gene& p2); //option to setMate - "ordered crossover"
        void setMateCX(Gene& p1, Gene& p2); //option to setMate - "cycle crossover"
        void copySetter(Gene& other);
        bool isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer);
        void print();


        virtual ~QueenGene() = default;
        vector<int> queen_rows;
    protected:

};

#endif // QUEENGENE_H
