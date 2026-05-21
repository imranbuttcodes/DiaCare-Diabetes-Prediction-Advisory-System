// src/recommendation.cpp
#include "../include/recommendation.h"
#include <iostream>
using namespace std;

// RISK LEVEL

string Recommendation::getRiskLevel(double prob) {
    if (prob < RISK_LOW) return "LOW";
    else if (prob < RISK_MODERATE_LOW) {
        return "MODERATE-LOW";
    } else if (prob < RISK_MODERATE) {
        return "MODERATE";
    } else if (prob < RISK_HIGH) {
        return "HIGH";
    } else {
        return "CRITICAL";
    }
}

// HEALTH RECOMMENDATION

string Recommendation::getRecommendation(double prob) {

    if (prob < RISK_LOW) {
        return REC_LOW;
    }
    else if (prob < RISK_MODERATE_LOW) {
        return REC_MODERATE_LOW;
    }
    else if (prob < RISK_MODERATE) {
        return  REC_MODERATE;
    }
    else if (prob < RISK_HIGH) {
        return REC_HIGH;
    }
    else {
        return REC_CRITICAL;
    }
}

// CRITICAL CHECK


bool Recommendation::isCritical(double prob) {
    return prob >= RISK_MODERATE;   
}

// CLL WEIGHT

int Recommendation::getCLLWeight(double prob) {
    // How many nodes in CLL for this patient
    if (prob >= 0.90){
         return 3;  
    } else if (prob >= 0.76) {
        return 2;
    } else if (prob >= 0.60) {
        return 1;
    } else {
        return 0;
    }   
}