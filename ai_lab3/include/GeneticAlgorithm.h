#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "Gene.h"
#include "Defs.h"

using namespace std;

class GeneticAlgorithm
{
    public:
        GeneticAlgorithm();

        static void init_population(vector<Gene*>& gene_vector);
        static void calc_fitness(vector<Gene*>& gene_vector);
        static void elitism(vector<Gene*>& gene_vector, vector<Gene*>& buffer, int esize);
        static void mate(vector<Gene*>& gene_vector, vector<Gene*>& buffer,
                         Mutate_type mutate_type = MUTATE_DEFAULT, Crossover_type x_type = CROSSOVER_DEFAULTX);
        static void mate_by_tournament(vector<Gene*>& gene_vector, vector<Gene*>& buffer, unsigned K,
                                       Mutate_type mutate_type = MUTATE_DEFAULT, Crossover_type x_type = CROSSOVER_DEFAULTX);
        static void mate_by_tournament_version2(vector<Gene*>& gene_vector, vector<Gene*>& buffer, unsigned K,
                                                Mutate_type mutate_type = MUTATE_DEFAULT, Crossover_type x_type = CROSSOVER_DEFAULTX);
        static int selectParentRws(vector<Gene*>& gene_vector);
        static void mate_rws(vector<Gene*>& gene_vector, vector<Gene*>& buffer,
                             Mutate_type mutate_type = MUTATE_DEFAULT, Crossover_type x_type = CROSSOVER_DEFAULTX);
        static bool compare_genes_ptr(Gene* lh, Gene* rh);
        static void print_best(vector<Gene*>& gene_vector);
        static void print_stats(vector<Gene*>& gene_vector);
        static void run_ga(vector<Gene*>& gene_vector, vector<Gene*>& buffer, MateType m_type = MT_DEFAULT,
                           Mutate_type mutate_type = MUTATE_DEFAULT, Crossover_type x_type = CROSSOVER_DEFAULTX);


    protected:

    private:
};

#endif // GENETICALGORITHM_H
