#include "Gene.h"
#include <math.h>

Gene::Gene(float fitness, float scaling_factor, int aging_factor, bool isUsingNiching, float sigma_share){
    this->fitness = fitness;
    this->age = 0;
    this->scaling_factor = scaling_factor;
    this->aging_factor = aging_factor;
    this->isUsingNiching = isUsingNiching;
    this->sigma_share = sigma_share;
}

void Gene::copySetter(Gene& other){
    this->age = other.age + this->aging_factor;
}

float Gene::getFitness(){
    return this->fitness;
}

bool Gene::local_optima_variance_signal(vector<Gene*>& gene_vector, float threshold){
    float tmp, mean = 0, variance = 0;

    for(unsigned int i=0; i<gene_vector.size(); i++)
        mean += gene_vector[i]->getFitness();
    mean /= gene_vector.size();

    for(unsigned int i=0; i<gene_vector.size(); i++)
        tmp = pow(gene_vector[i]->getFitness() - mean, 2);
        variance += tmp;
    variance /= gene_vector.size();

    return variance < threshold;
}

bool Gene::local_optima_gene_similar(vector<Gene*>& gene_vector, float threshold){
    //counter of how many times threshold succeeded, only when reaches 5 returns true.
    //if threshold succeeded, counter++
    //else, counter-- (but if reached back to zero, stay 0)
    static int counter = 0;

    int counter_limit = 5; //when counter reaches counter_limit, he starts returning true

    /*
    we will try to compute (not accurately) the overall distance between the genes.
    so we will just compute the average of distance between some of the pairs.
    */

    float sum = 0, num = 0;
    for(unsigned int i = 0; i < gene_vector.size() - 1; i++){ //all the <i,i+1> pairs
        sum += gene_vector[i]->dist(*gene_vector[i+1]);
    }
    num += (float)(gene_vector.size() - 1);

    for(unsigned int i = 0; i < gene_vector.size(); i++){ //all the <i, n-i> pairs
        sum += gene_vector[i]->dist(*gene_vector[gene_vector.size()-i]);
        num += 1.0f;
    }
    num += (float)(gene_vector.size());

    float average_estimation = sum / num;

    if(average_estimation < threshold)
        counter++;
    else if(counter > 0)
        counter--;

    return counter >= counter_limit;
}

float Gene::sharing_function(Gene& rh){
    float dij = this->dist(rh);
    if(dij < Gene::sigma_share)
        return 1.0f - dij / Gene::sigma_share; //to the power of 1 - fuck it
    else
        return 0.0f;
}

float Gene::sharing_fitness(float raw_fitness, vector<Gene*>& gene_vector){
    if(!Gene::isUsingNiching)
        return raw_fitness;
    else{
        float sum;
        for(unsigned int i = 0; i < gene_vector.size(); i++){
            sum += this->sharing_function(*gene_vector[i]);
        }
        return raw_fitness / sum;
    }
}
