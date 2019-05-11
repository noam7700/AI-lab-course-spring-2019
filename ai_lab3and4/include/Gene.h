#ifndef GENE_H
#define GENE_H

#include <vector>

using namespace std;

enum Mutate_type{
    MUTATE_DEFAULT = 0,
    MUTATE_SWAP = 1,
    MUTATE_SIM = 2, //simple inversion mutate
};

enum Crossover_type{
    CROSSOVER_DEFAULTX = 0,
    CROSSOVER_OX = 1, //ordered crossover (explained in word file)
    CROSSOVER_CX = 2, //cycle crossover (explained in word file)
    CROSSOVER_UNIFORM = 3, //uniform crossover (explained in ppt/wiki)
    CROSSOVER_TWOPOINT = 4 //two point crossover (explained in ppt/wiki)
};

enum LocalOptimaCombat_type{
    LocalOptimaCombat_superMutations = 0, //double the rate of mutations
    LocalOptimaCombat_immigrantion = 1, //add immigrants to replace native genes
    LocalOptimaCombat_niching = 2 //niching!
};

enum SignalMethod{
    SignalMethod_variance = 0,
    SignalMethod_similiar = 1
};

class Gene
{
    public:
        Gene(float fitness, float scaling_factor, int aging_factor, bool isUsingNiching, float sigma_share);

        virtual bool operator<(Gene& rh) = 0; //for sorting

        //they're void because they're setters of "this". To save #allocations (saving time)
        virtual void init() = 0; //init as random gene
        virtual void calc_fitness(vector<Gene*>& gene_vector) = 0; //update calc_fitness
        virtual void mutate(Mutate_type mutype = MUTATE_DEFAULT) = 0;
        virtual void setMate(Gene& p1, Gene& p2, Crossover_type xtype = CROSSOVER_DEFAULTX) = 0; //set attributes as the son of p1 & p2
        virtual void copySetter(Gene& other) = 0; //we assume this & other are the same type
        virtual bool isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer) = 0; //using fitness. (maybe fitness==0?)
        virtual void print() = 0; //for fun :P
        virtual float dist(Gene& rh) = 0;
        virtual bool local_optima_variance_signal(vector<Gene*>& gene_vector);
        virtual bool local_optima_gene_similar(vector<Gene*>& gene_vector);
        virtual float sharing_function(Gene& rh);
        virtual float sharing_fitness(float raw_fitness, vector<Gene*>& gene_vector);

        float getFitness();

        virtual ~Gene() = default;

    protected:
        float fitness;
        float scaling_factor; //will be 1 when not used
        int age; //will be always 0 when not used
        int aging_factor; //if age always grows by 3, age_factor=3 (age += age_factor in copySetter)
        bool isUsingNiching;
        float sigma_share;
};

#endif // GENE_H
