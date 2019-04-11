#include "GeneticAlgorithm.h"

#include <iostream>
#include <algorithm>
#include <stdlib.h> //used for rand


using namespace std;

#define GA_POPSIZE		2048		// ga population size
#define GA_MAXITER		16384		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE

GeneticAlgorithm::GeneticAlgorithm()
{
    //ctor
}


//assuming gene_vector doesn't have NULL cells
void GeneticAlgorithm::init_population(vector<Gene*>& gene_vector){
    for(unsigned int i=0; i<gene_vector.size(); i++){
        gene_vector[i]->init();
    }
}

//assuming gene_vector doesn't have NULL cells
void GeneticAlgorithm::calc_fitness(vector<Gene*>& gene_vector){
    for(unsigned int i=0; i<gene_vector.size(); i++){
        gene_vector[i]->calc_fitness();
    }
}

//assuming gene_vector & buffer don't have NULL cells
void GeneticAlgorithm::elitism(vector<Gene*>& gene_vector, vector<Gene*>& buffer, int esize){
    for(int i=0; i<esize; i++){
        buffer[i]->copySetter(*gene_vector[i]); //copy his attributes
    }
}

//assuming gene_vector & buffer don't have NULL cells
void GeneticAlgorithm::mate(vector<Gene*>& gene_vector, vector<Gene*>& buffer){
    int esize = GA_POPSIZE * GA_ELITRATE;
    elitism(gene_vector, buffer, esize);

    // Mate the rest
    Gene* child;
    int i1, i2;
    for(int i=esize; i<GA_POPSIZE; i++){
        i1 = rand() % (GA_POPSIZE / 2);
		i2 = rand() % (GA_POPSIZE / 2);
        child = buffer[i]; //it's pointers. we need to update buffer[i]
        child->setMate(*gene_vector[i1], *gene_vector[i2]);
        if(rand() < GA_MUTATION)
            child->mutate();
    }
}

bool GeneticAlgorithm::compare_genes_ptr(Gene* lh, Gene* rh){
    return (*lh < *rh);
}

void GeneticAlgorithm::print_best(vector<Gene*>& gene_vector){
    //assuming gene_vector is sorted
    gene_vector[0]->print();
}

//he receives the vectors with the proper derived objects of Gene
void GeneticAlgorithm::run_ga(vector<Gene*>& gene_vector, vector<Gene*>& buffer){
    //create random genes for all population. assuming gene_vector has non-NULLs
    init_population(gene_vector);

    for(int i=0; i<GA_MAXITER; i++){
        calc_fitness(gene_vector); //update fitness for all genes
        sort(gene_vector.begin(), gene_vector.end(), compare_genes_ptr);
        print_best(gene_vector);

        if(gene_vector[0]->isFinished(gene_vector, buffer))
            break;
        mate(gene_vector, buffer);
        swap(gene_vector, buffer);
    }

    return;
}





