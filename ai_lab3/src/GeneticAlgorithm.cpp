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

void GeneticAlgorithm::mate_by_tournament(vector<Gene*>& gene_vector, vector<Gene*>& buffer, unsigned K) {
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
        child->setMate(*gene_vector[esize], *gene_vector[esize+1]);
        if(rand() < GA_MUTATION)
            child->mutate();
    }
}

void GeneticAlgorithm::mate_by_tournament_version2(vector<Gene*>& gene_vector, vector<Gene*>& buffer, unsigned K) {
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

        child->setMate(*gene_vector[esize], *(*p2_it));

        if(rand() < GA_MUTATION)
            child->mutate();
    }
}

int GeneticAlgorithm::selectParentRws(vector<Gene*>& gene_vector){
    float sum = 0;
    for(unsigned int i=0; i<gene_vector.size(); i++)
        sum += gene_vector[i]->getFitness();

    float r = (float)(rand() % (int)sum);
    float curr_sum = 0;
    int parent_index;
    for(parent_index=0; parent_index<gene_vector.size() - 1; parent_index++){ //if reaches .size() - 1, just return him...
        curr_sum += sum - gene_vector[parent_index]->getFitness(); //now smaller getFitness, is better propability

        //assuming r >= prev_sum, propability of r < curr_sum: is r < curr_f.
        //this is exacly what we need if: f is better, when he's bigger
        if(r < curr_sum)
            break;
    }

    return parent_index;
}

void GeneticAlgorithm::mate_rws(vector<Gene*>& gene_vector, vector<Gene*>& buffer){
    int esize = GA_POPSIZE * GA_ELITRATE;
    elitism(gene_vector, buffer, esize);

    // Mate the rest
    Gene* child;
    int i1, i2;
    for(int i=esize; i<GA_POPSIZE; i++){
        i1 = selectParentRws(gene_vector);
		i2 = selectParentRws(gene_vector);
        child = buffer[i]; //it's pointers. we need to update buffer[i]
        child->setMate(*gene_vector[i1], *gene_vector[i2]);
        if(rand() < GA_MUTATION)
            child->mutate();
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

    cout << "mean: " << mean << ", variance: " << variance << ", standard deviation: " << st_deviation << "\n";
}

//he receives the vectors with the proper derived objects of Gene
void GeneticAlgorithm::run_ga(vector<Gene*>& gene_vector, vector<Gene*>& buffer, MateType m_type /*=MT_DEFAULT*/){
    auto mini_start = chrono::high_resolution_clock::now();
    auto mini_finish = chrono::high_resolution_clock::now();
    auto start = chrono::high_resolution_clock::now();
    auto finish = chrono::high_resolution_clock::now();
    std::chrono::duration<double,milli> elapsed = mini_finish - mini_start;

    clock_t cmini_start = clock(), cmini_finish, cstart = clock(), cfinish;
    float clockticks;

    //create random genes for all population. assuming gene_vector has non-NULLs
    init_population(gene_vector);

    for(int i=0; i<GA_MAXITER; i++){
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
			case MT_DEFAULT: mate(gene_vector, buffer); break;
			case MT_TOURNAMENT: mate_by_tournament(gene_vector, buffer, GA_TOURNAMENT_SIZE ); break;
			case MT_RWS: mate_rws(gene_vector, buffer); break;
			default: { cout << "Unknown mate type was used" << endl; return; }
        }
        swap(gene_vector, buffer);
    }

    finish = chrono::high_resolution_clock::now();
    elapsed = finish - start;
    cfinish = clock();
    clockticks = cfinish - cstart;
    cout << "Overall: absolute time: " << elapsed.count() / 1000 << " clock ticks: " << clockticks << "\n";

    return;
}





