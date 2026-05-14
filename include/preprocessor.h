#pragma once

#include "config.h"


class Preprocessor {
private:

    double impute_Insulin;
    double impute_SkinThickness;
    double impute_Glucose;
    double impute_BMI;
    double impute_BloodPressure;

    double cap_lower[TOTAL_RAW_FEATURES];
    double cap_upper[TOTAL_RAW_FEATURES];


    double scaler_mean[NUM_FEATURES];
    double scaler_std[NUM_FEATURES];

    bool isFitted;

    void   sortArray(double* arr, int n);
    double getPercentile(double* arr, int n, double p);
    double getNonZeroMedian(double* arr, int n);

    void  imputeRow(RawRow& row);
    double clip(double val, double lower, double hi);
    void capRow(RawRow& row);
    ProcessedRow engineerFeatures(const RawRow& row);
    void scaleRow(ProcessedRow& row);

public:
    Preprocessor();


    void fit(RawRow* trainData, int n);

    ProcessedRow transform(RawRow row);

    void fitTransform(RawRow* trainData, int n, ProcessedRow* output);

    void transformBatch(RawRow* data, int n, ProcessedRow* output);

    void printImputeParams();
    void printCapParams();
    void printScalerParams();
};