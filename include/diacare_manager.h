#pragma once

#include "config.h"
#include "patient.h"
#include "queue.h"
#include "doubly_linked_list.h"
#include "circular_linked_list.h"
#include "stack.h"
#include "BST.h"
#include "recommendation.h"
#include "csv_parser.h"
#include "train_test_split.h"
#include "preprocessor.h"
#include "logistic_regression.h"
#include "metrics.h"
#include "ML_PipeLine.h"

// DIACARE MANAGER
// Central controller
// Owns all DS structures + ML pipeline

class DiaCareManager {
private:
    
    ML_Pipeline pipeline;

    // Data Structures
    Queue intakeQueue;
    DoublyLinkedList activeWard;
    CircularLinkedList criticalMonitor;
    Stack actionHistory;
    BST patientIndex;

    // id countri
    int nextPatientId;

    string getCurrentDate();
    void displayMenu();
    void admitToWard(Patient* patient);
    void removeFromWard(int id);
    void enterVitals(double features[TOTAL_RAW_FEATURES]);

    void showBootScreen();
    void showReadyScreen();
    void registerPatient();
    void processNextPatient();
    void newCheckup();
    void dischargePatient();
    void undoLastAction();
    void whatIfSimulation();

    
    void viewWard();
    void navigateWard();
    void viewPatientHistory();
    void viewRiskTrajectory();
    void monitorCriticalPatients();
    void viewQueue();
    void viewActionHistory();
    void searchPatient();

public:

    DiaCareManager();
    ~DiaCareManager();

    bool startup(bool debug = false);

    
    void run();
    
};