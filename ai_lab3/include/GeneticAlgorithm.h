#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <vector>
#include "Gene.h"

using namespace std;

class GeneticAlgorithm
{
    public:
        GeneticAlgorithm();

        static void init_population(vector<Gene*>& gene_vector);
        static void calc_fitness(vector<Gene*>& gene_vector);
        static void elitism(vector<Gene*>& gene_vector, vector<Gene*>& buffer, int esize);
        static void mate(vector<Gene*>& gene_vector, vector<Gene*>& buffer);
        static bool compare_genes_ptr(Gene* lh, Gene* rh);
        static void print_best(vector<Gene*>& gene_vector);
        static void run_ga(vector<Gene*>& gene_vector, vector<Gene*>& buffer);


    protected:

    private:
};

#endif // GENETICALGORITHM_H