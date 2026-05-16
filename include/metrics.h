#pragma once

#include "config.h"
#include "preprocessor.h"

class Metrics {
private:
    int TP, TN, FP, FN;
    int total;

    double accuracy;
    double precision;
    double recall;
    double f1;

    bool isComputed;

public:
    Metrics();

    void compute(int* predicted, int* actual, int n);

    void printConfusionMatrix();
    void printMetrics();
    void printAll(string label);

    double getAccuracy()  { 
        return accuracy;  
    }
    double getPrecision() { 
        return precision; 
    }
    double getRecall() { 
        return recall;    
    }
    double getF1() {
         return f1;        
        }
    int getTP() {
         return TP;        
        }
    int getTN() {
         return TN;        
        }
    int getFP() {
         return FP;        
        }
    int getFN() {
         return FN;        
    }
};