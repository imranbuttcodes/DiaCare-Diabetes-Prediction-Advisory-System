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



const int EPOCHS = 1000;
const double LEARNING_RATE = 0.1;
const double THRESHOLD = 0.46;
const double LAMBDA = 0.04; // for l2 regularization


// Risk Thresholds 
const double RISK_LOW = 0.20;
const double RISK_MODERATE_LOW = 0.40;
const double RISK_MODERATE = 0.60;
const double RISK_HIGH = 0.80;



const string REC_LOW = 
   "Low diabetes risk.\n"
    "   -> Maintain healthy weight and BMI.\n"
    "   -> Limit sugar and refined carbs.\n"
    "   -> Stay physically active daily.\n"
    "   -> Annual checkup sufficient.";

const string REC_MODERATE_LOW = 
    "Mild risk factors detected.\n"
    "   -> Reduce sugar and processed food intake.\n"
    "   -> 30 minutes of walking daily recommended.\n"
    "   -> Monitor your weight and BMI.\n"
    "   -> Checkup every 6 months.";

const string REC_MODERATE = 
   "Notable diabetes risk detected.\n"
    "   -> Consult a doctor within 2 weeks.\n"
    "   -> Request a blood test.\n"
    "   -> Begin home glucose monitoring.\n"
    "   -> Switch to low glycemic index foods.";

const string REC_HIGH = 
   "High diabetes risk. Action needed.\n"
    "   -> Visit doctor this week.\n"
    "   -> Get fasting blood glucose and blood tests.\n"
    "   -> Begin structured diabetes management plan.\n"
    "   -> Monitor blood glucose daily.";

const string REC_CRITICAL = 
   "Critical diabetes risk. Urgent action required.\n"
    "   -> See doctor TODAY.\n"
    "   -> Immediate blood glucose test needed.\n"
    "   -> Medication may be required. Do not delay.";



    // DETERIORATION THRESHOLD
const float RAPID_DETERIORATION = 0.10;

// Action Type

const int ACTION_REGISTERED = 0;
const int ACTION_ADMITTED = 1;
const int ACTION_DISCHARGED = 2;
const int ACTION_CHECKUP_ADDED = 3;