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
    string option_algorithm = argv[1];
    string option_selection = argv[2]; //also known as m_type in our code. can be everything
    string option_mute = argv[3]; //if N Queens can be 0/1/2. otherwise 0
    string option_crossover = argv[4]; //if Bulls & Cows, can be 0/3/4. if N Queens, can be 0/1/2. otherwise 0

    int option_algorithm_int = atoi(option_algorithm.c_str());
    int option_selection_int = atoi(option_selection.c_str());
    int option_mute_int = atoi(option_mute.c_str());
    int option_crossover_int = atoi(option_crossover.c_str());

    cout << "Sample solution:" << endl << endl;

    vector<Gene*> gene_vector(GA_POPSIZE), buffer(GA_POPSIZE);
    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene(1.0f, 0.0f);
        buffer[i] = new StringGene(1.0f, 0.0f);
    }
    if(option_algorithm == "0"){

        MateType mt = static_cast<MateType>(option_selection_int);
        GeneticAlgorithm::run_ga(gene_vector, buffer, mt);
    }

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "Bulls and Cows:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGeneBullsAndCows(1.0f, 0.0f);
        buffer[i] = new StringGeneBullsAndCows(1.0f, 0.0f);
    }

    if(option_algorithm == "1"){
        MateType mt = static_cast<MateType>(option_selection_int);
        Crossover_type ct = static_cast<Crossover_type>(option_crossover_int);
        GeneticAlgorithm::run_ga(gene_vector, buffer, mt, MUTATE_DEFAULT, ct);
    }

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "Tournament:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene(1.0f, 0.0f);
        buffer[i] = new StringGene(1.0f, 0.0f);
    }
    if(option_algorithm == "2"){
        GeneticAlgorithm::run_ga(gene_vector, buffer, MT_TOURNAMENT);
    }
    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "RWS + scaling:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene(0.2f, 0.0f);
        buffer[i] = new StringGene(0.2f, 0.0f);
    }
    if(option_algorithm == "3"){
        GeneticAlgorithm::run_ga(gene_vector, buffer, MT_RWS);
    }
    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "Aging:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene(1.0f, 1.0f);
        buffer[i] = new StringGene(1.0f, 1.0f);
    }

    if(option_algorithm == "4"){
        MateType mt = static_cast<MateType>(option_selection_int);
        GeneticAlgorithm::run_ga(gene_vector, buffer, mt);
    }
    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "N Queens Problem:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){ //100 queens
        gene_vector[i] = new QueenGene(1.0f, 0.0f, 100);
        buffer[i] = new QueenGene(1.0f, 0.0f, 100);
    }

    if(option_algorithm == "5"){
        MateType mt = static_cast<MateType>(option_selection_int);
        Mutate_type mutet = static_cast<Mutate_type>(option_mute_int);
        Crossover_type ct = static_cast<Crossover_type>(option_crossover_int);
        GeneticAlgorithm::run_ga(gene_vector, buffer, mt, mutet, ct);
    }
    clean_vector(gene_vector);
    clean_vector(buffer);


    cout << endl << "N Queens Minimal conflicts:" << endl << endl;

    QueenMinimalConflicts qmc(100); //creates his random permutation board on the way.

    if(option_algorithm == "6")
        qmc.solve(1000); //MAX_IT=1000

    endProcedure();
	return 0;
}
