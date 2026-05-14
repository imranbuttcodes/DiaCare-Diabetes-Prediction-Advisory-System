#include <iostream>
using namespace std;


const string DATASET_PATH = "data/diabetes.csv";

const int NUM_FEATURES = 7;

const int TOTAL_RAW_FEATURES_INC_LABEL = 9;

const int TOTAL_RAW_FEATURES = 8;

const int MAX_ROWS = 800;

const int lower_percentile = 5.0;

const int higher_percentile = 95.0;

const string FEATURE_NAMES[NUM_FEATURES] = {
    "Pregnancies",
    "SkinThickness",
    "Insulin",
    "BMI",
    "Pedigree",
    "Glucose_BMI",
    "Glucose_Age"
};


struct RawRow {
    double Pregnancies;
    double Glucose;
    double BloodPressure;
    double SkinThickness;
    double Insulin;
    double BMI;
    double Pedigree;        
    double Age;
    int label;           
};


struct ProcessedRow {
    
    double features[NUM_FEATURES];
    
    int label;

};




