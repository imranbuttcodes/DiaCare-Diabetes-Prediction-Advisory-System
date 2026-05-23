// main.cpp
// #include <iostream>
// #include <string>

// #include "include/config.h"
// #include "include/csv_parser.h"
// #include "include/train_test_split.h"
// #include "include/preprocessor.h"
// #include "include/logistic_regression.h"
// #include "include/metrics.h"
#include "include/diacare_manager.h"
using namespace std;

int main() {

    // cout << "\n" << string(42, '=') << "\n";
    // cout << "  DiaCare - Diabetes Prediction System\n";
    // cout << string(42, '=') << "\n\n";

    // RawRow rawData[MAX_ROWS];
    // int n = 0;

    // bool loaded = CSVParser::load(DATASET_PATH, rawData, n);

    // if (!loaded) {
    //     cout << "Failed to load dataset. Exiting.\n";
    //     return 1;
    // }

    // cout << "Dataset loaded: " << n << " rows\n\n";


    // RawRow trainData[MAX_ROWS];
    // RawRow testData[MAX_ROWS];
    // int trainSize = 0;
    // int testSize  = 0;

    // TrainTestSplit::split(rawData,   n,trainData, trainSize, testData,  testSize, 0.2);

    // cout << "Train size: " << trainSize << endl;
    // cout << "Test size:  " << testSize  << endl;



    // Preprocessor preprocessor;

    // ProcessedRow processedTrain[MAX_ROWS];
    // ProcessedRow processedTest[MAX_ROWS];

    // preprocessor.fitTransform(trainData, trainSize, processedTrain);

    // preprocessor.transformBatch(testData, testSize, processedTest);


    // LogisticRegression model;
    // model.fit(processedTrain, trainSize);
    // model.printWeights();


    // int predTrain[MAX_ROWS],  actualTrain[MAX_ROWS];
    // int predTest[MAX_ROWS],   actualTest[MAX_ROWS];

    // for (int i = 0; i < trainSize; i++) {
    //     predTrain[i] = model.predict(processedTrain[i].features);
    //         actualTrain[i] = processedTrain[i].label;
    // }

    // for (int i = 0; i < testSize; i++) {
    //     predTest[i] = model.predict(processedTest[i].features);
    //     actualTest[i] = processedTest[i].label;
    // }

    // Metrics trainMetrics;
    // trainMetrics.compute(predTrain, actualTrain, trainSize);
    // trainMetrics.printAll("Training Set");

    // Metrics testMetrics;
    // testMetrics.compute(predTest, actualTest, testSize);
    // testMetrics.printAll("Test Set");


    // cout << "\n" << string(42, '=') << "\n";
    // cout << " Patient Prediction Test\n";
    // cout << string(42, '=') << "\n";

    // char again = 'y';

    // while (again == 'y' || again == 'Y') {

    //     RawRow input;
    //     input.label = -1;  

    //     cout << "Enter patient details" << endl;
    //     cout << string(42, '-') << endl;

    //     cout << "Pregnancies: ";
    //     cin  >> input.Pregnancies;

    //     cout << "Glucose (mg/dL) : ";
    //     cin  >> input.Glucose;

    //     cout << "Blood Pressure (mmHg) : ";
    //     cin  >> input.BloodPressure;

    //     cout << "Skin Thickness (mm) : ";
    //     cin  >> input.SkinThickness;

    //     cout << "Insulin (mu U/ml) : ";
    //     cin  >> input.Insulin;

    //     cout << "BMI : ";
    //     cin  >> input.BMI;

    //     cout << "Diabetes Pedigree : ";
    //     cin  >> input.Pedigree;

    //     cout << "Age : ";
    //     cin  >> input.Age;


    //     ProcessedRow processed = preprocessor.transform(input);


    //     double probability = model.predict_prob(processed.features);

    //     int prediction = model.predict(processed.features);

    //     cout << string(42, '-') << endl;
    //     cout << "PREDICTION RESULT" << endl;
    //     cout << string(42, '-') << "\n";
    //     cout << "  Probability : " << probability * 100 << endl;
    //     cout << "  Prediction   : " << (prediction == 1 ? "DIABETIC" : "NOT DIABETIC") << endl;


    //     string risk;
    //     if (probability < 0.20)
    //     {
    //         risk = "LOW";
    //     } else if (probability < 0.40) {
    //         risk = "MODERATE-LOW";
    //     }
    //     else if (probability < 0.60) {
    //         risk = "MODERATE";
    //     }
    //     else if (probability < 0.80) {
    //         risk = "HIGH";
    //     }
    //     else                         {
    //         risk = "CRITICAL";
    //     }

    //     cout << "Risk Level : " << risk << endl;
    //     cout << string(42, '-') << endl;

    //     cout << "Test another patient? (y/n): ";
    //     cin  >> again;
    // }

    // cout << "Exiting DiaCare. Goodbye endl";

    DiaCareManager mgr;
    mgr.startup();
    mgr.run();
    
    return 0;
}