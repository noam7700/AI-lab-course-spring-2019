#ifndef SRC_KNAPSACKGENE_H_
#define SRC_KNAPSACKGENE_H_

#include "Gene.h"

class KnapsackGene: public Gene {
public:
	// items[i] is a pair of <weight,price>
	static vector<pair<int, int>> items;

	KnapsackGene(double max_weight);

    virtual bool operator<(Gene& rh); //for sorting
    virtual void init(); //init as random gene
    virtual void calc_fitness(); //update calc_fitness
    virtual void mutate(Mutate_type mutype = MUTATE_DEFAULT);
    virtual void setMate(Gene& p1, Gene& p2, Crossover_type xtype = CROSSOVER_DEFAULTX); //set attributes as the son of p1 & p2
    virtual void copySetter(Gene& other); //we assume this & other are the same type
    virtual bool isFinished(vector<Gene*>& gene_vector, vector<Gene*>& buffer); //using fitness. (maybe fitness==0?)
    virtual void print(); //for fun :P

	virtual ~KnapsackGene() = default;

protected:
	// if knapsack[i]==true, items[i] is taken, otherwise it isn't
	vector<bool> knapsack;
	int max_weight;
	int weight;
	int value;
};

#endif /* SRC_KNAPSACKGENE_H_ */
