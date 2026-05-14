#include "../include/preprocessor.h"
#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace std;

Preprocessor::Preprocessor() : isFitted(false) {
    for (int i = 0; i < NUM_FEATURES; i++) {
        scaler_mean[i] = 0.0;
        scaler_std[i]  = 1.0;
    }
    for (int i = 0; i < 8; i++) {
        cap_lower[i] = 0.0;
        cap_upper[i] = 9999.0;
    }
    impute_Insulin       = 0.0;
    impute_SkinThickness = 0.0;
    impute_Glucose       = 0.0;
    impute_BMI           = 0.0;
    impute_BloodPressure = 0.0;
}


void Preprocessor::sortArray(double* arr, int n) {
    
    for(int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j + 1] < arr[j]) {
                double temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
        
    }
}

double Preprocessor::getPercentile(double* sortedArr, int n, double p) {

    double idx = (p / 100.0) * (n - 1);
    int lower  = (int)idx;
    int hi  = lower + 1;

    if (hi >= n) return sortedArr[n - 1];

    double frac = idx - lower;
    return sortedArr[lower] + frac * (sortedArr[hi] - sortedArr[lower]);
}

double Preprocessor::getNonZeroMedian(double* arr,int n) {
    // Collect non-zero values only
    
    double nonZero[MAX_ROWS];
    
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i] != 0.0)
            nonZero[count++] = arr[i];
    }

    if (count == 0) return 0.0;

    sortArray(nonZero, count);
    return getPercentile(nonZero, count, 50.0);
}


void Preprocessor::imputeRow(RawRow& row) {
    // Replace impossible zeros with
    // learned non-zero medians
    // Pregnancies = 0 is VALID so we'll never impute
    if (row.Insulin == 0.0)
        row.Insulin = impute_Insulin;
    if (row.SkinThickness == 0.0)
        row.SkinThickness = impute_SkinThickness;
    if (row.Glucose == 0.0)
        row.Glucose = impute_Glucose;
    if (row.BMI == 0.0)
        row.BMI = impute_BMI;
    if (row.BloodPressure == 0.0)
        row.BloodPressure = impute_BloodPressure;
}



double Preprocessor::clip(double val, double lower, double hi) {
    if (val < lower)    return lower;
    if (val > hi )  return hi;

    return val;
}


void Preprocessor::capRow(RawRow& row) {
    // Cap each feature to learned bounds
    // Index: 0=Preg, 1=Gluc, 2=BP, 3=Skin
    //        4=Ins,  5=BMI,  6=Pedigree, 7=Age


    row.Pregnancies = clip(row.Pregnancies, cap_lower[0], cap_upper[0]);
    row.Glucose = clip(row.Glucose, cap_lower[1], cap_upper[1]);
    row.BloodPressure = clip(row.BloodPressure, cap_lower[2], cap_upper[2]);
    row.SkinThickness = clip(row.SkinThickness, cap_lower[3], cap_upper[3]);
    row.Insulin = clip(row.Insulin, cap_lower[4], cap_upper[4]);
    row.BMI = clip(row.BMI, cap_lower[5], cap_upper[5]);
    row.Pedigree = clip(row.Pedigree, cap_lower[6], cap_upper[6]);
    row.Age = clip(row.Age, cap_lower[7], cap_upper[7]);
}


ProcessedRow Preprocessor::engineerFeatures(const RawRow& row) {

    ProcessedRow p;
    p.label = row.label;

    // Engineer interaction features
    double glucose_bmi = row.Glucose * row.BMI;
    double glucose_age = row.Glucose / (row.Age + 1.0);

    // Select final 7 features
    // Drop: Glucose, Age, BloodPressure
    p.features[0] = row.Pregnancies;   
    p.features[1] = row.SkinThickness; 
    p.features[2] = row.Insulin;       
    p.features[3] = row.BMI;           
    p.features[4] = row.Pedigree;      
    p.features[5] = glucose_bmi;       
    p.features[6] = glucose_age;       

    return p;
}


void Preprocessor::scaleRow(ProcessedRow& row) {
    for (int i = 0; i < NUM_FEATURES; i++) {
        if (scaler_std[i] != 0.0) {
            row.features[i] =  (row.features[i] - scaler_mean[i]) / scaler_std[i];
        }
    }
}


void Preprocessor::fit(RawRow* trainData, int n) {

    double insulin_arr[MAX_ROWS];
    double skin_arr[MAX_ROWS];
    double glucose_arr[MAX_ROWS];
    double bmi_arr[MAX_ROWS];
    double bp_arr[MAX_ROWS];

    for (int i = 0; i < n; i++) {

        insulin_arr[i] = trainData[i].Insulin;
        skin_arr[i] = trainData[i].SkinThickness;
        glucose_arr[i] = trainData[i].Glucose;
        bmi_arr[i] = trainData[i].BMI;
        bp_arr[i] = trainData[i].BloodPressure;
    }

    impute_Insulin = getNonZeroMedian(insulin_arr, n);
    impute_SkinThickness = getNonZeroMedian(skin_arr, n);
    impute_Glucose = getNonZeroMedian(glucose_arr, n);
    impute_BMI = getNonZeroMedian(bmi_arr, n);
    impute_BloodPressure = getNonZeroMedian(bp_arr, n);

    cout << "Imputation medians learned" << endl;


    double cols[TOTAL_RAW_FEATURES][MAX_ROWS];

    for (int i = 0; i < n; i++) {

        RawRow temp = trainData[i];

        // Apply learned imputation
        imputeRow(temp);

        // Store cleaned values column-wise
        cols[0][i] = temp.Pregnancies;
        cols[1][i] = temp.Glucose;
        cols[2][i] = temp.BloodPressure;
        cols[3][i] = temp.SkinThickness;
        cols[4][i] = temp.Insulin;
        cols[5][i] = temp.BMI;
        cols[6][i] = temp.Pedigree;
        cols[7][i] = temp.Age;
    }

    // Compute percentile caps
    for (int j = 0; j < TOTAL_RAW_FEATURES; j++) {

        double temp[MAX_ROWS];

        for (int i = 0; i < n; i++) {
            temp[i] = cols[j][i];
        }

        sortArray(temp, n);

        cap_lower[j] = getPercentile(temp, n, lower_percentile);

        cap_upper[j] = getPercentile(temp,n,higher_percentile);
    }

    cout << "Outlier caps learned" << endl;



    ProcessedRow engineered[MAX_ROWS];

    for (int i = 0; i < n; i++) {

        // Temporary row copy
        RawRow temp = trainData[i];

        // Apply preprocessing pipeline
        imputeRow(temp);
        capRow(temp);

        // Engineer final features
        engineered[i] = engineerFeatures(temp);
    }

    cout << "Temporary preprocessing complete" << endl;



    for (int j = 0; j < NUM_FEATURES; j++) {

        double sum = 0.0;

        for (int i = 0; i < n; i++) {
            sum += engineered[i].features[j];
        }

        scaler_mean[j] = sum / n;
    }

    cout << "Scaler means learned" << endl;


    // learn scler variance

    for (int j = 0; j < NUM_FEATURES; j++) {

        double variance = 0.0;

        for (int i = 0; i < n; i++) {

            double diff = engineered[i].features[j] - scaler_mean[j];

            variance += diff * diff;
        }

        scaler_std[j] = sqrt(variance / n);

        // Prevent divide-by-zero
        if (scaler_std[j] == 0.0) {
            scaler_std[j] = 1.0;
        }
    }

    cout << "Scaler stds learned" << endl;


    isFitted = true;

    cout << string(40, '-') << endl;
    cout << "Preprocessor fitting complete" << endl;
}


ProcessedRow Preprocessor::transform(RawRow row) {

    if (!isFitted) {
        throw runtime_error("Preprocessor not fitted.");
    }

    imputeRow(row);           
    capRow(row);              
    ProcessedRow p = engineerFeatures(row);
    scaleRow(p);              
    return p;
}


void Preprocessor::fitTransform(RawRow* trainData, int n, ProcessedRow* output) {

    fit(trainData, n);
    for (int i = 0; i < n; i++) {
        output[i] = transform(trainData[i]);
    }
        
}

void Preprocessor::transformBatch(RawRow* data, int n, ProcessedRow* output) {

    if (!isFitted) {
        throw runtime_error( "Preprocessor not fitted.");
    }


    for (int i = 0; i < n; i++) {
        output[i] = transform(data[i]);
    }
        
}


void Preprocessor::printImputeParams() {
    cout << "\nImputation Values (non-zero medians) " << endl;
    cout << "Feature                Value"  << endl;
    cout << string(30, '-') << endl;
    cout << "Insulin            " << impute_Insulin << endl;
    cout << "SkinThickness      "  << impute_SkinThickness << endl;
    cout << "Glucose            "  << impute_Glucose<< endl;
    cout << "BMI                "  << impute_BMI << endl;
    cout << "BloodPressure      "  << impute_BloodPressure << endl;
}

void Preprocessor::printCapParams() {
    string names[TOTAL_RAW_FEATURES] = {
        "Pregnancies", "Glucose",
        "BloodPressure", "SkinThickness",
        "Insulin", "BMI", "Pedigree", "Age"
    };
    cout << "\nOutlier Caps (5th - 95th percentile) "<< endl;
    cout << "Feature      Lower       Upper" << endl;;
    cout << string(40, '-') << endl;
    for (int j = 0; j < TOTAL_RAW_FEATURES; j++) {
        cout << names[j] << "       "  << cap_lower[j] << "         " <<  cap_upper[j] << endl;
    }
}

void Preprocessor::printScalerParams() {
    cout << "\nScaler Params (7 final features) " << endl;
    cout  << "Feature        Mean       Std\n";
    cout << string(44, '-') << "\n";
    for (int j = 0; j < NUM_FEATURES; j++) {
        cout << FEATURE_NAMES[j] << "      " << scaler_mean[j] << "      " <<  scaler_std[j] << endl;
    }
}

