// src/patient.cpp
#include "../include/patient.h"
#include <iostream>
using namespace std;


Patient::Patient(): id(0), age(0), probability(0.0), historyHead(nullptr), visitCount(0) {}


Patient::Patient(int id, string name, int age): id(id), name(name), 
age(age), probability(0.0), historyHead(nullptr), visitCount(0) {}


Patient::~Patient() {
    deleteHistory(historyHead);
    historyHead = nullptr;
}

// Recursively deletes entire SLL
void Patient::deleteHistory(CheckupRecord* node) {
    if (node == nullptr) return;

    deleteHistory(node->next);
    delete node;
}


void Patient::addCheckup(double rawFeatures[TOTAL_RAW_FEATURES], double prob, string date) {

    CheckupRecord* newRecord   = new CheckupRecord();
    newRecord->visitNumber = ++visitCount;
    newRecord->date = date;
    newRecord->probability = prob;
    newRecord->riskLevel = Recommendation::getRiskLevel(prob);
    newRecord->recommendation  = Recommendation::getRecommendation(prob);
    newRecord->next = nullptr;

    for (int i = 0; i < TOTAL_RAW_FEATURES; i++) {
        newRecord->rawFeatures[i] = rawFeatures[i];
    }
        
    // Update patient's latest prediction 
    probability = prob;
    riskLevel = newRecord->riskLevel;
    recommendation = newRecord->recommendation;


    if (historyHead == nullptr) {
        historyHead = newRecord;
        return;
    }

    CheckupRecord* temp = historyHead;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
        
    temp->next = newRecord;
}


void Patient::displayHistory() {
    if (historyHead == nullptr) {
        cout << "  No checkup history found.\n";
        return;
    }

    cout << "\n  Checkup History: " << name << "\n";
    cout << "-----------------------------------------------------" << endl;
    cout << "Visit  |  Date        |  Risk          |  Probability  " << endl;
    cout << "-----------------------------------------------------" << endl;

    CheckupRecord* temp = historyHead;
    while (temp != nullptr) {
        cout << temp->visitNumber << "      "  << temp->date << "      "  << temp->riskLevel << "           "  << temp->probability * 100 << "%" << endl;
        temp = temp->next;
    }
    cout << "-----------------------------------------------------" << endl;
}

// RISK TRAJECTORY: Recursive SLL Analysis

double Patient::getRiskTrajectory() {
    // Need at least 2 visits to calculate slope
    if (visitCount < 2) return 0.0;

    // Find last 3 checkup nodes
    CheckupRecord* visits[3] = {nullptr, nullptr, nullptr};
    CheckupRecord* temp = historyHead;

    while (temp != nullptr) {
        visits[0] = visits[1];
        visits[1] = visits[2];
        visits[2] = temp;
        temp = temp->next;
    }

    // Count how many we actually found
    int n = 0;
    for (int i = 0; i < 3; i++) {
        if (visits[i] != nullptr){
            n++;
        }
    }
        
    if (n < 2) return 0.0;

    // Find first non-null
    int start = 0;
    while (visits[start] == nullptr) {
        start++;
    }

    // Slope = (last prob - first prob) / n
    double first = visits[start]->probability;
    double last  = visits[2]->probability;

    return (last - first) / n;
}

bool Patient::isRapidDeterioration() {
    // Risk increasing more than RAPID_DETERIORATION per visit
    return getRiskTrajectory() > RAPID_DETERIORATION;
}

// DEEP COPY: Full clone including SLL

Patient* Patient::deepCopy() {
    Patient* copy = new Patient(id, name, age);
    copy->probability = probability;
    copy->riskLevel = riskLevel;
    copy->recommendation = recommendation;
    copy->visitCount = 0;

    // Deep copy entire SLL
    CheckupRecord* temp = historyHead;
    while (temp != nullptr) {
        copy->addCheckup(
            temp->rawFeatures,
            temp->probability,
            temp->date
        );
        temp = temp->next;
    }

    return copy;
}


// DISPLAY SUMMARY:  One line per patient

void Patient::displaySummary() {
    cout << "[" << id << "]  " << "Name: " << name << "  Age: " << age << "  P: "   << probability * 100 << "%" << "  Risk: " << riskLevel << "  Visits: " << visitCount << endl;
}


// DISPLAY FULL: Complete patient details

void Patient::displayFull() {
    cout << "\n" << string(42, '=') << endl;
    cout << "Patient Details" << endl;
    cout << string(42, '=') << endl;
    cout << "ID : " << id << endl;
    cout << "Name : " << name << endl;
    cout << "Age : " << age << endl;
    cout << "Risk Level : " << riskLevel << endl;
    cout << "P(Diabetes): " << probability * 100 << "%" << endl;


    if (isRapidDeterioration()) {
        cout << "\n!! RAPID DETERIORATION DETECTED !!" << endl;
        cout << "Risk is rising significantly across recent visits." << endl;
        cout << "Immediate medical attention strongly advised." << endl;
    }

    cout << "\nRecommendation:" << endl;
    cout << recommendation << endl;
    cout << string(42, '=') << endl;

    displayHistory();
}

// ACTION RECORD

ActionRecord::ActionRecord(): type(0), snapshot(nullptr) {}

ActionRecord::ActionRecord(int t, Patient* snap):type(t), snapshot(snap) {}

ActionRecord::~ActionRecord() {
    // snapshot is a deep copy: we own it
    if (snapshot != nullptr) {
        delete snapshot;
        snapshot = nullptr;
    }
}