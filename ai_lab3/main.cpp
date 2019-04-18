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
#include "QueenGene.h"
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
    srand(time(0)); //seed the rand only once (according to https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/)
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
    //GeneticAlgorithm::run_ga(gene_vector, buffer, MT_RWS);

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "Aging:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGeneAging();
        buffer[i] = new StringGeneAging();
    }

    //GeneticAlgorithm::run_ga(gene_vector, buffer, MT_DEFAULT);

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "N Queens Problem:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){ //100 queens
        gene_vector[i] = new QueenGene(100);
        buffer[i] = new QueenGene(100);
    }

    GeneticAlgorithm::run_ga(gene_vector, buffer, MT_DEFAULT, MUTATE_SIM, CROSSOVER_OX);

    clean_vector(gene_vector);
    clean_vector(buffer);


    /*QueenGene* test1 = new QueenGene(10); //0-9
    test1->queen_rows = {8,2,4,3,7,5,1,0,9,6};
    QueenGene* test2 = new QueenGene(10); //0-9
    test2->queen_rows = {4,1,7,6,2,8,3,9,5,0};

    cout << "perm1: (";
    for(unsigned int i=0; i<test2->queen_rows.size(); i++)
        cout << test1->queen_rows[i] << " ";
    cout << ")\n";
    cout << "perm2: (";
    for(unsigned int i=0; i<test2->queen_rows.size(); i++)
        cout << test2->queen_rows[i] << " ";
    cout << ")\n";


    QueenGene* spring = new QueenGene(10); //0-9
    spring->setMateOX(*test1, *test2);
    cout << "perm3: (";
    for(unsigned int i=0; i<10; i++)
        cout << spring->queen_rows[i] << " ";
    cout << ")\n";*/

	return 0;
}
