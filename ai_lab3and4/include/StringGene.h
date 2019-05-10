#ifndef STRINGGENE_H
#define STRINGGENE_H

#include <string>
#include <Gene.h>

using namespace std;

class StringGene : public Gene
{
    public:
        StringGene(float scaling_factor, int aging_factor); //init will set str to random

        static string target;

        bool operator<(Gene& rh); //for sorting
        void init(); //init as random gene
        void calc_fitness(); //update calc_fitness
        void mutate(Mutate_type mutype = MUTATE_DEFAULT);
        void setMate(Gene& p1, Gene& p2, Crossover_type xtype = CROSSOVER_DEFAULTX); //set attributes as the son of p1 & p2
        void copySetter(Gene& other); //we assume this & other are the same type
        bool isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer); //using fitness. (maybe fitness==0?)
        void print();
        float dist(Gene& rh);

        ~StringGene() = default;

    protected:
        string str;
};

#endif // STRINGGENE_H
