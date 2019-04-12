// Genetic5.cpp : Defines the entry point for the console application.
//

#ifdef CHECK

int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	return 0;
}

#endif

#pragma warning(disable:4786)		// disable debug warning

#include <iostream>					// for cout etc.
#include <vector>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <time.h>					// for random seed
#include <math.h>

#include "Gene.h"
#include "StringGeneBullsAndCows.h"
#include "geneticAlgorithm.h"

#define GA_POPSIZE		2048		// ga population size
#define GA_MAXITER		16384		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")

using namespace std;				// polluting global namespace, but hey...

void clean_vector(vector<Gene*> vec)
{
    for(int i=0; i<GA_POPSIZE; i++)
    {
        delete vec[i];
        vec[i] = NULL;
    }
}

int main()
{
    vector<Gene*> gene_vector(GA_POPSIZE), buffer(GA_POPSIZE);
    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene();
        buffer[i] = new StringGene();
    }
    GeneticAlgorithm::run_ga(gene_vector, buffer);

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGeneBullsAndCows();
        buffer[i] = new StringGeneBullsAndCows();
    }
    GeneticAlgorithm::run_ga(gene_vector, buffer);

    clean_vector(gene_vector);
    clean_vector(buffer);

	return 0;
}
