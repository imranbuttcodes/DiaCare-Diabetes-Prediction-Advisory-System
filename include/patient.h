// include/patient.h
#pragma once

#include "config.h"
#include "recommendation.h"


// CHECKUP RECORD

// One visit node: SLL inside Patient


struct CheckupRecord {
    int visitNumber;
    string date;
    double rawFeatures[TOTAL_RAW_FEATURES];
    double probability;
    string riskLevel;
    string recommendation;

    CheckupRecord* next;

    CheckupRecord(): visitNumber(0), probability(0.0), next(nullptr) {
        for (int i = 0; i < TOTAL_RAW_FEATURES; i++) {
            rawFeatures[i] = 0.0;
        }
            
    }
};



class Patient;

// ACTION RECORD: Stored in Stack for undo mechanism

struct ActionRecord {
    int type;      
    Patient* snapshot;    // deep copy of patient

    ActionRecord();
    ActionRecord(int type, Patient* snapshot);
    ~ActionRecord();
};



class Patient {
private:

    // Recursive helpers
    void deleteHistory(CheckupRecord* node);

public:


    int id;
    string name;
    int age;

    // Latest Prediction 
    // Updated every time addCheckup() is called
    // Used for quick display without SLL traversal

    double probability;
    string riskLevel;
    string recommendation;

    
    CheckupRecord* historyHead;
    int visitCount;


    Patient();
    Patient(int id, string name, int age);
    ~Patient();


    void addCheckup(double rawFeatures[TOTAL_RAW_FEATURES], double probability, string date);

    void displayHistory();

    // Risk Trajectory (recursive SLL)
    double getRiskTrajectory();
    bool   isRapidDeterioration();

        Patient* deepCopy();

    void displaySummary();
    void displayFull();
};

