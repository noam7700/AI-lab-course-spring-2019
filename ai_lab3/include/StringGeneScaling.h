#ifndef STRINGGENESCALING_H
#define STRINGGENESCALING_H
#include "StringGene.h"

class StringGeneScaling: public StringGene{
    public:
        StringGeneScaling();
        void calc_fitness(); //update calc_fitness

        virtual ~StringGeneScaling() = default;
};

#endif // STRINGGENESCALING_H
