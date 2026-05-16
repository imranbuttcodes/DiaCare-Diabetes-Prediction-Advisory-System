#pragma once

#include "config.h"
#include "preprocessor.h"

class LogisticRegression {
private:
    double weights[NUM_FEATURES];
    double bias;
    double posWeight;
    double negWeight;
    
    bool isTrained;

    double sigmoid(double z);

public:
    LogisticRegression();

    void fit(ProcessedRow* data, int n);
    double predict_prob(double* features);   // probability
    int predict(double* features);   // 0 or 1

    void printWeights();
    
    bool trained() {
         return isTrained; 
    }
    
    double getWeight(int i) {
         return weights[i]; 
    }
    
    double getBias() { 
        return bias;       
    }
};