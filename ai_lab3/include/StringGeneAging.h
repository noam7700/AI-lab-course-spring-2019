#ifndef STRINGGENEAGING_H
#define STRINGGENEAGING_H
#include "StringGene.h"

class StringGeneAging: public StringGene
{
    public:
        StringGeneAging();
        void calc_fitness(); //update calc_fitness
        void init(); //init as random gene
        void copySetter(Gene& other); //we assume this & other are the same type
        void setMate(Gene& p1, Gene& p2, Crossover_type xtype = CROSSOVER_DEFAULTX); //set attributes as the son of p1 & p2

        virtual ~StringGeneAging() = default;

    protected:
        int age;
};

#endif // STRINGGENEAGING_H
