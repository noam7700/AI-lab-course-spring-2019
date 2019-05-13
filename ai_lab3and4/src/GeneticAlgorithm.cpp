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

bool GeneticAlgorithm::compare_genes_ptr(Gene* lh, Gene* rh){
    return (*lh < *rh);
}

void GeneticAlgorithm::print_best(vector<Gene*>& gene_vector){
    //assuming gene_vector is sorted
    gene_vector[0]->print();
}

int GeneticAlgorithm::selectParentRws(vector<Gene*>& gene_vector){
    //we want rws but when fi is smaller, we want bigger chance.
    //therefore, zi=1-fi/sum, and do the regular algorithm for zi.
    //when fi is smaller, 1-fi/sum is bigger, and 1-fi/sum is positive, so no problem with the regular algorithm
    //we will improve this idea by taking zi=1 - delta * fi/sum, so the zi wont be so close to each other.
    //also, we need to normalize zi to sum to 1 (or some bigger const) so we don't get everyone at 99% to be selected
    float real_sum = 0;
    float delta = 500; //seems to give average results
    for(unsigned int i=0; i<gene_vector.size(); i++)
        real_sum += gene_vector[i]->getFitness();

    //we need new_sum so we can normalize fi's to sum to 10,000 (not 1, because we want to use rand())
    float new_sum = 0, ratio_fit;
    for(unsigned int i=0; i<gene_vector.size(); i++){
        ratio_fit = gene_vector[i]->getFitness()/real_sum;
        new_sum += 1 - delta * ratio_fit;
    }

    float r = (float)(rand() % 10000);
    float zi, curr_sum = 0;
    unsigned int parent_index;
    for(parent_index=0; parent_index<gene_vector.size(); parent_index++){
        ratio_fit = gene_vector[parent_index]->getFitness()/real_sum;
        zi = (1 - delta * ratio_fit) / new_sum * 10000; //now zi is in [0,10000)
        curr_sum += zi;
        if(r < curr_sum)
            break;
    }
    return parent_index;
}

//re-arrange gene_vector and puts the parents in esize, esize+1. therefore, returns pair<int,int>(esize,esize+1)
pair<int,int> GeneticAlgorithm::selectParentTournament(vector<Gene*>& gene_vector,
                                                       unsigned K/*= GA_TOURNAMENT_SIZE*/){
    int esize = GA_POPSIZE * GA_ELITRATE;


    random_shuffle(gene_vector.begin() + esize, gene_vector.begin() + GA_POPSIZE); //shuffle[esize,end)
    sort(gene_vector.begin() + esize, gene_vector.begin() + esize + K, compare_genes_ptr); //sort[esize, esize+K]

    return pair<int,int>(esize, esize+1);
}

pair<int,int> GeneticAlgorithm::selectParentTournament_version2(vector<Gene*>& gene_vector,
                                                                unsigned K /*= GA_TOURNAMENT_SIZE*/){

    int esize = GA_POPSIZE * GA_ELITRATE;

    vector<Gene*>::iterator p1_it, p2_it;
    p1_it = min_element(gene_vector.begin() + esize, gene_vector.begin() + esize + K, compare_genes_ptr);

    //put p1 at gene_vector[esize] so we can find p2 easily with min_element(...)
    swap(gene_vector[esize], *p1_it);

    // Second winning gene
    p2_it = min_element(gene_vector.begin() + esize + 1, gene_vector.begin() + esize + K, compare_genes_ptr);
    //put p2 at gene_vector[esize+1] just because it's nicer
    swap(gene_vector[esize+1], *p2_it);

    return pair<int,int>(esize, esize+1);
}

int GeneticAlgorithm::selectParentShay(vector<Gene*>& gene_vector){
    return rand() % (GA_POPSIZE / 2);
}

pair<int,int> GeneticAlgorithm::selectParent(vector<Gene*>& gene_vector, MateType m_type /*= MT_DEFAULT*/, unsigned K /*= 0*/){
    int i1, i2;
    pair<int,int> tmp;
    switch(m_type){
			case MT_DEFAULT:
			    i1 = selectParentShay(gene_vector);
			    i2 = selectParentShay(gene_vector);
			    break;
			case MT_TOURNAMENT:
                tmp = selectParentTournament(gene_vector, K);
                i1 = tmp.first;
                i2 = tmp.second;
			    break;
			case MT_RWS:
                i1 = selectParentRws(gene_vector);
                i2 = selectParentRws(gene_vector);
			    break;
			default: { cout << "Error: Unknown mate type was used" << endl; throw; }
    }
    return pair<int,int>(i1, i2);
}

void GeneticAlgorithm::generic_mate(vector<Gene*>& gene_vector, vector<Gene*>& buffer,
                         Mutate_type mutate_type /*= MUTATE_DEFAULT*/,
                         Crossover_type x_type /*= CROSSOVER_DEFAULTX*/, MateType m_type /*= MT_DEFAULT*/,
                         unsigned K /*= GA_TOURNAMENT_SIZE*/, bool local_optima_signal /*= false*/,
                         LocalOptimaCombat_type loc_type /*= LocalOptimaCombat_superMutations*/){

    int esize = GA_POPSIZE * GA_ELITRATE;
    elitism(gene_vector, buffer, esize);

    // Mate the rest
    Gene* child;
    int i1, i2;
    pair<int, int> tmp;
    for(int i=esize; i<GA_POPSIZE; i++){
        tmp = selectParent(gene_vector, m_type); //can change order of gene in vector (tournament does)
        i1 = tmp.first;
		i2 = tmp.second;
        child = buffer[i]; //it's pointers. we need to update buffer[i]
        child->setMate(*gene_vector[i1], *gene_vector[i2], x_type);
        if(local_optima_signal && loc_type == LocalOptimaCombat_superMutations){
            if(rand() < 2 * GA_MUTATION) //double the chances
                child->mutate(mutate_type);
        }
        else if(rand() < GA_MUTATION) //act normal
            child->mutate(mutate_type);
    }

    //random immigration, if needed
    if(local_optima_signal && loc_type == LocalOptimaCombat_immigrantion){
        for(int i=0; i<esize/5; i++){ //fifth of the elitism pop' replaced with immigrants (2% overall)
            buffer[i]->calc_fitness(); //replace him with random immigrant
        }
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
                              Mutate_type mutate_type /*=MUTATE_DEFAULT*/, Crossover_type x_type /*=CROSSOVER_DEFAULT*/,
                              SignalMethod sm /*= SignalMethod_variance*/,
                              LocalOptimaCombat_type loc_type /*= LocalOptimaCombat_superMutations*/){
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

    //local optima signal & combat
    bool local_optima_signal = false;

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

        //now create new generation

        //check for 'local optima signal'. if do, combat local optima!
        if(sm == SignalMethod_variance)
            local_optima_signal = gene_vector[0]->local_optima_variance_signal(gene_vector);
        else if(sm == SignalMethod_similiar)
            local_optima_signal = gene_vector[0]->local_optima_gene_similar(gene_vector);
        else{
            cout << "Error: no such local optima signal.\n";
            throw;
        }

        //niching is just changing the fitnesses values.
        if(local_optima_signal == true && loc_type == LocalOptimaCombat_niching)
            gene_vector[0]->add_niching_to_fitness(gene_vector);

        generic_mate(gene_vector, buffer, mutate_type, x_type, m_type, GA_TOURNAMENT_SIZE, local_optima_signal, loc_type);

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
        cout << "We succeeded at generation " << i << "\n";

    return;
}





