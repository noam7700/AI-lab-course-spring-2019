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
    string option_selection = argv[2]; //can be 0/1/2
    string option_mute = argv[3]; //if N Queens can be 0/1/2. otherwise 0
    string option_crossover = argv[4]; //if Bulls & Cows, can be 0/3/4. if N Queens, can be 0/1/2. otherwise 0
    string option_signalmethod = argv[5]; //can be 0/1
    string option_localcombat = argv[6]; //can be 0/1/2

    //get option as int (for enums...)
    int option_selection_int = atoi(option_selection.c_str());
    int option_mute_int = atoi(option_mute.c_str());
    int option_crossover_int = atoi(option_crossover.c_str());
    int option_signalmethod_int = atoi(option_signalmethod.c_str());
    int option_localcombat_int = atoi(option_localcombat.c_str());

    MateType mt = static_cast<MateType>(option_selection_int);
    Mutate_type mutet = static_cast<Mutate_type>(option_mute_int);
    Crossover_type ct = static_cast<Crossover_type>(option_crossover_int);
    SignalMethod sigm = static_cast<SignalMethod>(option_signalmethod_int);
    LocalOptimaCombat_type loc_type = static_cast<LocalOptimaCombat_type>(option_localcombat_int);

    bool isUsingNiching = (loc_type == LocalOptimaCombat_niching);
    //sigma for each problem is not decided by the user.
    float sigma_QueenGene100 = 30.0f;//because when dij>30, <i,j> are not worth punishing (they can benifit from each other)
    string target = "Hello sir, I'm Noam Blagovsky and this is partner, Yuri khvoles";
    float sigma_StringGeneHELLOWORLD = target.size() * 0.5f; //in avg, each index(letter) is dist=1

    cout << "Sample solution:" << endl << endl;

    vector<Gene*> gene_vector(GA_POPSIZE), buffer(GA_POPSIZE);
    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene(1.0f, 0.0f, isUsingNiching, sigma_StringGeneHELLOWORLD);
        buffer[i] = new StringGene(1.0f, 0.0f, isUsingNiching, sigma_StringGeneHELLOWORLD);
    }
    if(option_algorithm == "0"){

        GeneticAlgorithm::run_ga(gene_vector, buffer, mt, mutet, ct, sigm, loc_type);
    }

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "Bulls and Cows:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGeneBullsAndCows(1.0f, 0.0f, isUsingNiching, sigma_StringGeneHELLOWORLD);
        buffer[i] = new StringGeneBullsAndCows(1.0f, 0.0f, isUsingNiching, sigma_StringGeneHELLOWORLD);
    }

    if(option_algorithm == "1"){
        GeneticAlgorithm::run_ga(gene_vector, buffer, mt, mutet, ct, sigm, loc_type);
    }

    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "Aging with strings:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){
        gene_vector[i] = new StringGene(1.0f, 1.0f, isUsingNiching, sigma_StringGeneHELLOWORLD);
        buffer[i] = new StringGene(1.0f, 1.0f, isUsingNiching, sigma_StringGeneHELLOWORLD);
    }

    if(option_algorithm == "2"){
        GeneticAlgorithm::run_ga(gene_vector, buffer, mt, mutet, ct, sigm, loc_type);
    }
    clean_vector(gene_vector);
    clean_vector(buffer);

    cout << endl << "N Queens Problem:" << endl << endl;

    for(int i=0; i<GA_POPSIZE; i++){ //100 queens
        gene_vector[i] = new QueenGene(1.0f, 0.0f, 100, isUsingNiching, sigma_QueenGene100);
        buffer[i] = new QueenGene(1.0f, 0.0f, 100, isUsingNiching, sigma_QueenGene100);
    }

    if(option_algorithm == "3"){
        GeneticAlgorithm::run_ga(gene_vector, buffer, mt, mutet, ct);
    }
    clean_vector(gene_vector);
    clean_vector(buffer);


    cout << endl << "N Queens Minimal conflicts:" << endl << endl;

    QueenMinimalConflicts qmc(100); //creates his random permutation board on the way.

    if(option_algorithm == "4")
        qmc.solve(1000); //MAX_IT=1000


    endProcedure();
	return 0;
}
