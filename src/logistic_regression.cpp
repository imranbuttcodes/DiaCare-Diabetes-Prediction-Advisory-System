#include "../include/logistic_regression.h"
#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace std;

LogisticRegression::LogisticRegression(): bias(0.0), isTrained(false) {
    for (int i = 0; i < NUM_FEATURES; i++) {
        weights[i] = 0.0;
    }
    posWeight = 0.0;
    negWeight = 0.0;
}


double LogisticRegression::sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}


void LogisticRegression::fit(ProcessedRow* data, int n) {

    cout << string(42, '=') << endl;
    cout << "Training Logistic Regression "<< endl; 
    cout << string(42, '=') << endl;
    cout << "Samples:       " << n << endl;
    cout << "Features:      " << NUM_FEATURES  << endl;
    cout << "Epochs:        " << EPOCHS << endl;
    cout << "Learning Rate: " << LEARNING_RATE << endl;
    cout << "Threshold:     " << THRESHOLD << endl;
    cout << string(42, '-') << endl << endl;

    //calculete class counts;
    int posCount = 0, negCount = 0;

    for (int i = 0; i < n; i++) {
        if (data[i].label == 1) {
            posCount++;
        } else {
            negCount++;
        }
    }
 
    if (posCount == 0 || negCount == 0) {
        throw runtime_error("Dataset must contain both classes");
    }
    
    posWeight = n / (2.0 * posCount);
    negWeight = n / (2.0 * negCount);

    // Gradient Descent Loop
    for (int epoch = 0; epoch < EPOCHS; epoch++) {

        double grad_of_weights[NUM_FEATURES] = {0};
        double grad_of_Bais = 0.0;

        for (int i = 0; i < n; i++) {

            double z = bias;
            for (int j = 0; j < NUM_FEATURES; j++) {
                z += weights[j] * data[i].features[j];
            }
            
            double yhat = sigmoid(z);

            double sample_Weight;

            if (data[i].label == 1) {
                sample_Weight = posWeight;
            } else {
                sample_Weight = negWeight;
            }

            double error = sample_Weight * (data[i].label - yhat);

            grad_of_Bais += error;
            for (int j = 0; j < NUM_FEATURES; j++) {
                grad_of_weights[j] += error * data[i].features[j];
            }
        }

        grad_of_Bais /= n;
        // l2 Reguralization 
        for (int j = 0; j < NUM_FEATURES; j++) {
            grad_of_weights[j] = (grad_of_weights[j] / n) - (LAMBDA * weights[j]);
        
        }
            
         // Update weights and bias 
        
        bias += LEARNING_RATE * grad_of_Bais;
        for (int j = 0; j < NUM_FEATURES; j++) {
            weights[j] += LEARNING_RATE * grad_of_weights[j];        
        }
    }

    isTrained = true;
    cout << "  Training complete" << endl;
    cout << string(42, '=') << endl;
}


double LogisticRegression::predict_prob(double* features) {

    if (!isTrained) {
        throw runtime_error("Model not trained.");
    }

    double z = bias;
    for (int j = 0; j < NUM_FEATURES; j++) {
        z += weights[j] * features[j];
    }
    return sigmoid(z);
}

int LogisticRegression::predict(double* features) {
    if (!isTrained) {
        throw runtime_error("Model not trained.");
    }
    return (predict_prob(features) >= THRESHOLD) ? 1 : 0;
}

void LogisticRegression::printWeights() {
    cout << "Learned Weights" << endl;
    cout << string(36, '-') << endl;
    cout << "Feature             Weight"  << endl;
    cout << string(36, '-') << endl;
    for (int j = 0; j < NUM_FEATURES; j++) {
        cout << FEATURE_NAMES[j] << "       " << weights[j] << endl;
    }

    cout << "Bias                   " << bias <<  endl;
    cout << string(36, '-') << endl<< endl;
}