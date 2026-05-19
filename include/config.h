#pragma once
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



const int EPOCHS = 5000;
const double LEARNING_RATE = 0.1;
const double THRESHOLD = 0.46;
const double LAMBDA = 0.04; // for l2 regularization


// Risk Thresholds 
const double RISK_LOW = 0.20;
const double RISK_MODERATE_LOW = 0.40;
const double RISK_MODERATE = 0.60;
const double RISK_HIGH = 0.80;

const int WARD_MAX_CAPACITY = 10;