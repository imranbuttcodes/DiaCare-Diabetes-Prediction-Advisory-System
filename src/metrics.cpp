#include "../include/metrics.h"
#include <iostream>
#include <iomanip>
using namespace std;


Metrics::Metrics():TP(0), TN(0), FP(0), FN(0), total(0), accuracy(0), precision(0), recall(0), f1(0), isComputed(false) {}


void Metrics::compute(int* predicted,int* actual,int  n) {

    TP = TN = FP = FN = 0;
    total = n;

    for (int i = 0; i < n; i++) {
        if (predicted[i]==1 && actual[i]==1) {
            TP++;
        } else if (predicted[i]==0 && actual[i]==0) {
            TN++; 
        } else if (predicted[i]==1 && actual[i]==0) {
            FP++;
        }
        else if (predicted[i]==0 && actual[i]==1){ 
            FN++;
        }
    }

    accuracy = (double)(TP + TN) / n * 100.0;

    // to avoid division by zero
    precision = (TP + FP > 0) ? (double)TP / (TP + FP) * 100.0 : 0.0;

    recall = (TP + FN > 0) ? (double)TP / (TP + FN) * 100.0 : 0.0;

    f1 = (precision + recall > 0) ? 2.0 * precision * recall / (precision + recall) : 0.0;

    isComputed = true;
}



void Metrics::printConfusionMatrix() {
    cout << "Confusion Matrix:" << endl;
    cout << "--------------------------------" << endl;
    cout << "               Predicted" << endl;
    cout << "                Pos     Neg" << endl;
    cout << "  Actual Pos  " << TP << "       " << FN << endl;
    cout << "  Actual Neg  " << FP << "       " << TN << endl;
    cout << "--------------------------------" << endl;
}

void Metrics::printMetrics() {
    cout << "  Samples:    " << total       << endl;
    cout << "  Accuracy:   " << accuracy    << endl;
    cout << "  Precision:  " << precision   << endl;
    cout << "  Recall:     " << recall      << endl;
    cout << "  F1 Score:   " << f1          << endl;
}

void Metrics::printAll(string label) {
    cout << string(23,'-') << endl;
    cout << label << endl;
    printConfusionMatrix();
    printMetrics();
    cout << string(23,'-') << endl;

}