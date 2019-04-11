#ifndef STRINGGENE_H
#define STRINGGENE_H

#include <string>
#include <Gene.h>

using namespace std;

class StringGene : public Gene
{
    public:
        StringGene(); //init will set str to random

        static string target;

        bool operator<(Gene& rh); //for sorting
        void init(); //init as random gene
        void calc_fitness(); //update calc_fitness
        void mutate();
        void setMate(Gene& p1, Gene& p2); //set attributes as the son of p1 & p2
        void copySetter(Gene& other); //we assume this & other are the same type
        bool isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer); //using fitness. (maybe fitness==0?)
        void print();

    private:
        string str;
};

#endif // STRINGGENE_H
