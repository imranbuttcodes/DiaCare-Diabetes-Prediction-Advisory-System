#pragma once
#include "config.h"


class Recommendation {
public:

    // Risk Level 
    static string getRiskLevel(double prob);

    //  Health Advice 
    static string getRecommendation(double prob);

    // Critical check (goes to CLL) 
    static bool isCritical(double prob);

    // CLL weight (how many nodes) 
    static int getCLLWeight(double prob);
};