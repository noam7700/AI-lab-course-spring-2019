#include "GeneticAlgorithm.h"

using namespace std;


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
void GeneticAlgorithm::mate(vector<Gene*>& gene_vector, vector<Gene*>& buffer,
                            Mutate_type mutate_type /*= MUTATE_DEFAULT*/, Crossover_type x_type /*= CROSSOVER_DEFAULTX*/){
    int esize = GA_POPSIZE * GA_ELITRATE;
    elitism(gene_vector, buffer, esize);

    // Mate the rest
    Gene* child;
    int i1, i2;
    for(int i=esize; i<GA_POPSIZE; i++){
        i1 = rand() % (GA_POPSIZE / 2);
		i2 = rand() % (GA_POPSIZE / 2);
        child = buffer[i]; //it's pointers. we need to update buffer[i]
        child->setMate(*gene_vector[i1], *gene_vector[i2], x_type);
        if(rand() < GA_MUTATION)
            child->mutate(mutate_type);
    }
}

bool GeneticAlgorithm::compare_genes_ptr(Gene* lh, Gene* rh){
    return (*lh < *rh);
}

void GeneticAlgorithm::print_best(vector<Gene*>& gene_vector){
    //assuming gene_vector is sorted
    gene_vector[0]->print();
}

void GeneticAlgorithm::mate_by_tournament(vector<Gene*>& gene_vector, vector<Gene*>& buffer, unsigned K,
                                          Mutate_type mutate_type /*= MUTATE_DEFAULT*/, Crossover_type x_type /*= CROSSOVER_DEFAULTX*/) {
    int esize = GA_POPSIZE * GA_ELITRATE;
    elitism(gene_vector, buffer, esize);

    // Mate the rest
    Gene* child;
    for(int i=esize; i < GA_POPSIZE; i++){

    	// First winning gene
        random_shuffle(gene_vector.begin() + esize, gene_vector.begin() + GA_POPSIZE) ;
        sort(gene_vector.begin() + esize, gene_vector.begin() + esize + K, compare_genes_ptr);
        // Second winning gene
		random_shuffle(gene_vector.begin() + esize + 1, gene_vector.begin() + GA_POPSIZE) ;
		sort(gene_vector.begin() + esize + 1, gene_vector.begin() + esize + K, compare_genes_ptr);

        child = buffer[i]; //it's pointers. we need to update buffer[i]
        child->setMate(*gene_vector[esize], *gene_vector[esize+1], x_type);
        if(rand() < GA_MUTATION)
            child->mutate(mutate_type);
    }
}

void GeneticAlgorithm::mate_by_tournament_version2(vector<Gene*>& gene_vector, vector<Gene*>& buffer, unsigned K,
                                                   Mutate_type mutate_type /*= MUTATE_DEFAULT*/, Crossover_type x_type /*= CROSSOVER_DEFAULTX*/) {
    int esize = GA_POPSIZE * GA_ELITRATE;
    elitism(gene_vector, buffer, esize);

    // Mate the rest
    Gene* child;
    vector<Gene*>::iterator p1_it, p2_it; //parents. p1 won the tournament, and p2 at second place
    int p2_index;
    for(int i=esize; i < GA_POPSIZE; i++){
    	if(K > gene_vector.size() - esize){
    		K = gene_vector.size() - esize;
    	}
    	random_shuffle(gene_vector.begin() + esize, gene_vector.end()); //we want to take random sub-group

        // First winning gene - in range [esize, esize + K) - sub-group size should be K
        p1_it = min_element(gene_vector.begin() + esize, gene_vector.begin() + esize + K, compare_genes_ptr);

        //put p1 at gene_vector[esize] so we can find p2 easily with min_element(...)
        swap(gene_vector[esize], *p1_it);
        // Second winning gene
		p2_it = min_element(gene_vector.begin() + esize + 1, gene_vector.begin() + esize + K, compare_genes_ptr);

        child = buffer[i]; //it's pointers. we need to update buffer[i]

        child->setMate(*gene_vector[esize], *(*p2_it), x_type);

        if(rand() < GA_MUTATION)
            child->mutate(mutate_type);
    }
}

int GeneticAlgorithm::selectParentRws(vector<Gene*>& gene_vector){
    //we want rws but when fi is smaller, we want bigger chance.
    //therefore, zi=e^(-fi), and do the regular algorithm for zi.
    //when fi is smaller, -fi is bigger, and e^(-fi) is positive, so no problem with the regular algorithm
    //also, can use zi = e^(-delta * fi / sum). delta is to control the numbers so they wont be too dense
    //delta1 needs to be bigger/smaller depends on "fi/sum", and the values of fi/sum depends on POP_SIZE usually,
    //so it's easier because POP_SIZE is fixed
    float real_sum = 0;
    float delta = 500; //seems to give average results
    for(unsigned int i=0; i<gene_vector.size(); i++)
        real_sum += gene_vector[i]->getFitness();

    //we need new_sum so we can normalize fi's to sum to 10,000 (not 1, because we want to use rand())
    float new_sum = 0, ratio_fit;
    for(unsigned int i=0; i<gene_vector.size(); i++){
        ratio_fit = gene_vector[i]->getFitness()/real_sum;
        new_sum += pow(2.71, -delta * ratio_fit);
    }

    float r = (float)(rand() % 10000);
    float zi, curr_sum = 0;
    unsigned int parent_index;
    for(parent_index=0; parent_index<gene_vector.size(); parent_index++){
        ratio_fit = gene_vector[parent_index]->getFitness()/real_sum;
        zi = pow(2.71, -delta * ratio_fit) / new_sum * 10000; //now zi is in [0,10000)
        curr_sum += zi;
        if(r < curr_sum)
            break;
    }
    return parent_index;
}

void GeneticAlgorithm::mate_rws(vector<Gene*>& gene_vector, vector<Gene*>& buffer,
                                Mutate_type mutate_type /*= MUTATE_DEFAULT*/, Crossover_type x_type /*= CROSSOVER_DEFAULTX*/){
    int esize = GA_POPSIZE * GA_ELITRATE;
    elitism(gene_vector, buffer, esize);

    // Mate the rest
    Gene* child;
    int i1, i2;
    for(int i=esize; i<GA_POPSIZE; i++){
        i1 = selectParentRws(gene_vector);
		i2 = selectParentRws(gene_vector);
        child = buffer[i]; //it's pointers. we need to update buffer[i]
        child->setMate(*gene_vector[i1], *gene_vector[i2], x_type);
        if(rand() < GA_MUTATION)
            child->mutate(mutate_type);
    }
}

void GeneticAlgorithm::print_stats(vector<Gene*>& gene_vector){
    float mean = 0, variance = 0, st_deviation;
    for(unsigned int i=0; i<gene_vector.size(); i++)
        mean += gene_vector[i]->getFitness();
    mean /= gene_vector.size();

    float tmp;
    for(unsigned int i=0; i<gene_vector.size(); i++)
        tmp = pow(gene_vector[i]->getFitness() - mean, 2);
        variance += tmp;
    variance /= gene_vector.size();

    st_deviation = sqrt(variance);

    cout << "mean: " << mean << ", standard deviation: " << st_deviation << "\n";
}

//he receives the vectors with the proper derived objects of Gene
void GeneticAlgorithm::run_ga(vector<Gene*>& gene_vector, vector<Gene*>& buffer, MateType m_type /*=MT_DEFAULT*/,
                              Mutate_type mutate_type /*=MUTATE_DEFAULT*/, Crossover_type x_type /*=CROSSOVER_DEFAULT*/){
    auto mini_start = chrono::high_resolution_clock::now();
    auto mini_finish = chrono::high_resolution_clock::now();
    auto start = chrono::high_resolution_clock::now();
    auto finish = chrono::high_resolution_clock::now();
    std::chrono::duration<double,milli> elapsed = mini_finish - mini_start;

    clock_t cmini_start = clock(), cmini_finish, cstart = clock(), cfinish;
    float clockticks;

    //create random genes for all population. assuming gene_vector has non-NULLs
    init_population(gene_vector);
    //sometimes we assume buffer already has well-defined attributes (for example strings in regular size)
    init_population(buffer);

    //code for checking if we reached to local minimum
    int rounds_till_failing = 100; //if we reach 100 rounds after last improvement we consider it as getting stuck
    float last_check_fit = (float)INT_MAX; //so in the first check we've definitely improved
    float current_check_fit;
    int didWeFail = 0, last_generation_improvement; //did we fail, and from which generation we got stuck

    //so we can show him how much time did it take to reach this local minimum
    auto last_improvement_mini_finish = chrono::high_resolution_clock::now();
    clock_t last_improvement_cmini_finish = clock();

    int i;
    for(i=0; i<GA_MAXITER; i++, rounds_till_failing--){
        calc_fitness(gene_vector); //update fitness for all genes
        sort(gene_vector.begin(), gene_vector.end(), compare_genes_ptr);
        print_best(gene_vector);
        print_stats(gene_vector);

        mini_finish = chrono::high_resolution_clock::now();
        elapsed = mini_finish - mini_start;
        cmini_finish = clock();
        clockticks = (float)(cmini_finish - cmini_start);

        cmini_start = cmini_finish; //switch between finish & start for next iteration
        mini_start = mini_finish; //same
        cout << "Generation " << i << ": absolute time: " << elapsed.count() / 1000 << " clock ticks: " << clockticks << "\n";
        cout << "\n";

        if(gene_vector[0]->isFinished(gene_vector, buffer))
            break;
        switch(m_type){
			case MT_DEFAULT: mate(gene_vector, buffer, mutate_type, x_type); break;
			case MT_TOURNAMENT: mate_by_tournament(gene_vector, buffer, GA_TOURNAMENT_SIZE, mutate_type, x_type); break;
			case MT_RWS: mate_rws(gene_vector, buffer, mutate_type, x_type); break;
			default: { cout << "Unknown mate type was used" << endl; return; }
        }

        //time for checking
        current_check_fit = gene_vector[0]->getFitness();
        if(current_check_fit < last_check_fit){ //we imroved
            last_improvement_mini_finish = chrono::high_resolution_clock::now(); //so if we get stuck we can show real time
            last_improvement_cmini_finish = clock();
            last_check_fit = current_check_fit;
            rounds_till_failing = 100;
        }
        if(rounds_till_failing == 0){
            didWeFail = 1;
            last_generation_improvement = i - 100; //obviously..
            break;
        }

        swap(gene_vector, buffer);
    }
    if(didWeFail == 1){
        finish = last_improvement_mini_finish;
        cfinish = last_improvement_cmini_finish;
    }
    else{
        finish = chrono::high_resolution_clock::now();
        cfinish = clock();
    }
    elapsed = finish - start;
    clockticks = cfinish - cstart;
    cout << "Overall: absolute time: " << elapsed.count() / 1000 << " clock ticks: " << clockticks << "\n";
    if(didWeFail == 1)
        cout << "We failed at generation " << last_generation_improvement << "\n";
    else
        cout << "We succeeded at generation " <<

    return;
}





