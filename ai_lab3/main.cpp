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

#include "Gene.h"
#include "StringGeneBullsAndCows.h"
#include "StringGeneScaling.h"
#include "StringGeneAging.h"
#include "geneticAlgorithm.h"
#include "Defs.h"

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
    cout << "Sample solution:" << endl << endl;
    vector<Gene*> gene_vector(GA_POPSIZE), buffer(GA_POPSIZE);
    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene();
        buffer[i] = new StringGene();
    }
    //GeneticAlgorithm::run_ga(gene_vector, buffer);

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "Bulls and Cows:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGeneBullsAndCows();
        buffer[i] = new StringGeneBullsAndCows();
    }
    //GeneticAlgorithm::run_ga(gene_vector, buffer);

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "Tournament:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene();
        buffer[i] = new StringGene();
    }
    //GeneticAlgorithm::run_ga(gene_vector, buffer, MT_TOURNAMENT);

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "RWS + scaling:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGeneScaling();
        buffer[i] = new StringGeneScaling();
    }
    GeneticAlgorithm::run_ga(gene_vector, buffer, MT_RWS);

    clean_vector(gene_vector);
    clean_vector(buffer);


	return 0;
}
