#ifndef GENE_H
#define GENE_H

#include <vector>

using namespace std;

enum mutate_type{
    MUTATE_DEFAULT = 0,
    MUTATE_SWAP = 1,
    MUTATE_SIM = 2, //simple inversion mutate
};

enum crossover_type{
    CROSSOVER_DEFAULTX = 0,
    CROSSOVER_OX = 1, //ordered crossover
    CROSSOVER_CX = 2 //cycle crossover
};

class Gene
{
    public:
        Gene(float fitness);

        virtual bool operator<(Gene& rh) = 0; //for sorting

        //they're void because they're setters of "this". To save #allocations (saving time)
        virtual void init() = 0; //init as random gene
        virtual void calc_fitness() = 0; //update calc_fitness
        virtual void mutate() = 0;
        virtual void setMate(Gene& p1, Gene& p2) = 0; //set attributes as the son of p1 & p2
        virtual void copySetter(Gene& other) = 0; //we assume this & other are the same type
        virtual bool isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer) = 0; //using fitness. (maybe fitness==0?)
        virtual void print() = 0; //for fun :P

        float getFitness();

        virtual ~Gene() = default;

    protected:
        float fitness;
};

#endif // GENE_H
