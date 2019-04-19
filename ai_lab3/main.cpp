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
#include "geneticAlgorithm.h"
#include "QueenGene.h"
#include "QueenMinimalConflicts.h"
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

void endProcedure(){
    while(1){
        cout << "Press 'f' to finish (w/o '')\n";
        string input;
        cin >> input;
        if(input == "f")
            break;
    }

}

int main(int argc, char *argv[])
{
    srand(time(0)); //seed the rand only once (according to https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/)

    //get option
    string first_arg = argv[1];



    cout << "Sample solution:" << endl << endl;

    vector<Gene*> gene_vector(GA_POPSIZE), buffer(GA_POPSIZE);
    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene(1.0f, 0.0f);
        buffer[i] = new StringGene(1.0f, 0.0f);
    }
    if(first_arg == "0")
        GeneticAlgorithm::run_ga(gene_vector, buffer);

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "Bulls and Cows:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGeneBullsAndCows(1.0f, 0.0f);
        buffer[i] = new StringGeneBullsAndCows(1.0f, 0.0f);
    }
    if(first_arg == "1")
        GeneticAlgorithm::run_ga(gene_vector, buffer, MT_DEFAULT, MUTATE_DEFAULT, CROSSOVER_UNIFORM);

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "Tournament:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene(1.0f, 0.0f);
        buffer[i] = new StringGene(1.0f, 0.0f);
    }
    if(first_arg == "2")
        GeneticAlgorithm::run_ga(gene_vector, buffer, MT_TOURNAMENT);

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "RWS + scaling:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene(0.2f, 0.0f);
        buffer[i] = new StringGene(0.2f, 0.0f);
    }
    if(first_arg == "3")
        GeneticAlgorithm::run_ga(gene_vector, buffer, MT_RWS);

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "Aging:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene(1.0f, 1.0f);
        buffer[i] = new StringGene(1.0f, 1.0f);
    }

    if(first_arg == "4")
        GeneticAlgorithm::run_ga(gene_vector, buffer, MT_DEFAULT);

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "N Queens Problem:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){ //100 queens
        gene_vector[i] = new QueenGene(1.0f, 0.0f, 100);
        buffer[i] = new QueenGene(1.0f, 0.0f, 100);
    }

    if(first_arg == "5")
        GeneticAlgorithm::run_ga(gene_vector, buffer, MT_DEFAULT, MUTATE_SWAP, CROSSOVER_CX);

    clean_vector(gene_vector);
    clean_vector(buffer);


    cout << endl << "N Queens Minimal conflicts:" << endl << endl;

    QueenMinimalConflicts qmc(100); //creates his random permutation board on the way.

    if(first_arg == "6")
        qmc.solve(1000); //MAX_IT=1000

    endProcedure();
	return 0;
}
