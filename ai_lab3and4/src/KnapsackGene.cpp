#include "KnapsackGene.h"

vector<pair<int, int>> KnapsackGene::items;

KnapsackGene::KnapsackGene(double max_weight): Gene(0), knapsack(vector<bool>()), max_weight(max_weight), weight(0), value(0) {}

bool KnapsackGene::operator<(Gene& rh) {
	return this->fitness < rh.getFitness();
}

void KnapsackGene::init() {
	srand((unsigned)time(NULL));
	for (size_t i = 0; i < items.size(); i++){
		bool b = static_cast<bool>(rand() % 2);
		knapsack.push_back(b);
	}
	calc_fitness();
}

void KnapsackGene::calc_fitness() {
	// fitness is an item price
	fitness = 1;
	weight = 0;
	value = 0;

	for (size_t i = 0; i < knapsack.size(); i++){
		if(knapsack[i]){
			value += KnapsackGene::items[i].first;
			weight += KnapsackGene::items[i].second;
		}
		// the knapsack is overweight, so we drop an item from the past
		while(weight > max_weight){
			int rnd_idx = rand() % i;
			knapsack[rnd_idx] = false;
			value -= KnapsackGene::items[rnd_idx].first;
			weight -= KnapsackGene::items[rnd_idx].second;
		}
		fitness /= value;
	}
}

void KnapsackGene::mutate(Mutate_type mutype) {
	int rnd_idx = rand() % knapsack.size();
	knapsack[rnd_idx] = ~knapsack[rnd_idx];
}

void KnapsackGene::setMate(Gene& p1, Gene& p2, Crossover_type xtype) {

	KnapsackGene& p1_casted = static_cast<KnapsackGene&>(p1);
	KnapsackGene& p2_casted = static_cast<KnapsackGene&>(p2);

    int cut = rand() % KnapsackGene::knapsack.size();

    vector<bool>::const_iterator first1 = p1_casted.knapsack.begin();
    vector<bool>::const_iterator last1 = p1_casted.knapsack.begin() + cut;
    vector<bool>::const_iterator first2 = p2_casted.knapsack.begin() + cut;
    vector<bool>::const_iterator last2 = p2_casted.knapsack.end();

    vector<bool> ks = vector<bool>(first1, last1);
    ks.insert(ks.end(), first2, last2);
    this->knapsack = ks;
}

void KnapsackGene::copySetter(Gene& other) {
	KnapsackGene& other_casted = static_cast<KnapsackGene&>(other); //assuming other is KnapsackGene
    //deep copy w/o allocations (I don't trust operator= of vectors because they might re-allocate the memory)
    for(unsigned int i=0; i<this->knapsack.size(); i++)
        this->knapsack[i] = other_casted.knapsack[i];
}

bool KnapsackGene::isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer) {
	return static_cast<KnapsackGene*>(gene_vector[0])->weight == max_weight;
}

void KnapsackGene::print() {
	cout << "Knapsack fitness: " << fitness << endl;
	cout << "Knapsack value: " << value << endl;
	cout << "Knapsack weight: " << weight << "/" << max_weight << endl;

	for (size_t i = 0; i < knapsack.size(); i++){
		if(knapsack[i]){
			cout << "(" << KnapsackGene::items[i].first << "$, " << KnapsackGene::items[i].second << "kg" << ")";
		}
	}
	cout << endl;
}
