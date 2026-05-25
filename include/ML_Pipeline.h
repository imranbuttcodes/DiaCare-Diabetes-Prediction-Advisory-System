#pragma once
#include "config.h"  
#include "logistic_regression.h"
#include "../include/metrics.h"


class ML_Pipeline {
private:
    RawRow rawData[MAX_ROWS];

    RawRow trainData[MAX_ROWS];
    RawRow testData[MAX_ROWS];

    int predTest[MAX_ROWS];
    int actualTest[MAX_ROWS];
    
    ProcessedRow processedTrain[MAX_ROWS];
    ProcessedRow processedTest[MAX_ROWS];
        
    int n;
    int trainSize;
    int testSize;

    bool isReady;

    Preprocessor preprocessor;
    LogisticRegression model;
    Metrics metrics;


    bool loadData();
    void splitData(double testRatio = 0.2);
    void preprocess();
    void train();
    void evaluate(bool debug);
    RawRow buildRawRow(double features[TOTAL_RAW_FEATURES]);


public:
    ML_Pipeline();

    
    int predict(double* features);
    double predict_prob(double* features);
    void printModelReport();
    bool run(bool debug);

    
};

