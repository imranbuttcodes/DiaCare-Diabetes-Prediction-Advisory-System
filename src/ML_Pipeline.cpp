#include "../include/ML_Pipeline.h"
#include "../include/csv_parser.h"
#include "../include/train_test_split.h"


ML_Pipeline::ML_Pipeline(){
    n = 0;
    trainSize = 0;
    testSize = 0;
    isReady = false;
}


bool ML_Pipeline::loadData() {
    cout << "Loading dataset..." << endl;

    if (!CSVParser::load(DATASET_PATH, rawData, n)) {
        cout << "Failed to load dataset." << endl;
        return false;
    }

    return true;
}

void ML_Pipeline::splitData(double testRatio) {
    cout << "Splitting dataset..." << endl;

    TrainTestSplit::split(
        rawData, n,
        trainData, trainSize,
        testData, testSize,
        testRatio
    );

    cout << "TrainSize: " << trainSize << " TestSize: " << testSize << endl;
}


void ML_Pipeline::preprocess() {
    preprocessor.fitTransform(trainData, trainSize, processedTrain);
    preprocessor.transformBatch(testData, testSize, processedTest);
}

void ML_Pipeline::train() {
    cout << "Training model..." << endl;

    model.fit(processedTrain, trainSize);
}

void ML_Pipeline::evaluate(bool debug) {
    cout << "Evaluating model..." << endl;

    for (int i = 0; i < testSize; i++) {
        predTest[i] = model.predict(processedTest[i].features);
        actualTest[i] = processedTest[i].label;
    }

    metrics.compute(predTest, actualTest, testSize);

    if (debug) {
        model.printWeights();
        metrics.printAll("Test Set");
    }
}


RawRow ML_Pipeline::buildRawRow(double features[TOTAL_RAW_FEATURES]) {

    RawRow raw;
    raw.Pregnancies   = features[0];
    raw.Glucose       = features[1];
    raw.BloodPressure = features[2];
    raw.SkinThickness = features[3];
    raw.Insulin       = features[4];
    raw.BMI           = features[5];
    raw.Pedigree      = features[6];
    raw.Age           = features[7];
    raw.label         = -1;
    return raw;
}



double ML_Pipeline::predict_prob(double* features) {

     if (!isReady) {
        cout << "Pipeline not initialized." << endl;
        return 0.0;
    }

    RawRow raw = buildRawRow(features);

    ProcessedRow processed = preprocessor.transform(raw);

    return model.predict_prob(processed.features);
}


int ML_Pipeline::predict(double* features) {

     if (!isReady) {
        cout << "Pipeline not initialized." << endl;
        return 0.0;
    }

    
    RawRow raw = buildRawRow(features);

    ProcessedRow processed = preprocessor.transform(raw);
    return model.predict(processed.features);
}




void ML_Pipeline::printModelReport() {
    cout << "\n============= MODEL REPORT =============" << endl;

    model.printWeights();
    metrics.printAll("Test Set");

    cout << "========================" << endl;;
} 



bool ML_Pipeline::run(bool debug) {

    if (!loadData()) {
        return false;
    }
        

    splitData(0.2);

    if (trainSize == 0 || testSize == 0) {
        cout << "Invalid dataset split." << endl;
        return false;
    }

    preprocess();
    train();
    

    evaluate(debug);
    
    isReady = true;

    return true;
}
