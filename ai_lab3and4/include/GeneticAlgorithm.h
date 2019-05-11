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
        //doesn't depend on selection method
        static void generic_mate(vector<Gene*>& gene_vector, vector<Gene*>& buffer,
                         Mutate_type mutate_type = MUTATE_DEFAULT,
                         Crossover_type x_type = CROSSOVER_DEFAULTX, MateType m_type = MT_DEFAULT,
                         unsigned K = GA_TOURNAMENT_SIZE, bool local_optima_signal = false,
                         LocalOptimaCombat_type loc_type = LocalOptimaCombat_superMutations);
        static pair<int,int> selectParent(vector<Gene*>& gene_vector, MateType m_type = MT_DEFAULT, unsigned K = GA_TOURNAMENT_SIZE);
        static int selectParentRws(vector<Gene*>& gene_vector);
        //must return both parents at once (pair<int,int> instaed of just int)
        static pair<int,int> selectParentTournament(vector<Gene*>& gene_vector, unsigned K = GA_TOURNAMENT_SIZE);
        static pair<int,int> selectParentTournament_version2(vector<Gene*>& gene_vector, unsigned K = GA_TOURNAMENT_SIZE);
        static int selectParentShay(vector<Gene*>& gene_vector);
        static bool compare_genes_ptr(Gene* lh, Gene* rh);
        static void print_best(vector<Gene*>& gene_vector);
        static void print_stats(vector<Gene*>& gene_vector);
        static void run_ga(vector<Gene*>& gene_vector, vector<Gene*>& buffer, MateType m_type = MT_DEFAULT,
                           Mutate_type mutate_type = MUTATE_DEFAULT, Crossover_type x_type = CROSSOVER_DEFAULTX,
                           SignalMethod sm = SignalMethod_variance,
                           LocalOptimaCombat_type loc_type = LocalOptimaCombat_superMutations);


    protected:

    private:
};

#endif // GENETICALGORITHM_H
