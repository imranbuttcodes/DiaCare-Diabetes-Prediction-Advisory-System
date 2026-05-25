#include <iostream>
#include <string>
#include <ctime>
#include "../include/diacare_manager.h"

using namespace std;


DiaCareManager::DiaCareManager(){
    nextPatientId = 1;
}

DiaCareManager::~DiaCareManager() {

    while(!activeWard.isEmpty()) {
        Patient* p = activeWard.removeLowestRisk();
        if (p != nullptr) {
            delete p ;
        }
    }
    
    while(!intakeQueue.isEmpty()) {
        Patient* p = intakeQueue.dequeue();
        if (p != nullptr) {
            delete p;
        }
        
    }
}

string DiaCareManager::getCurrentDate() {
    time_t now = time(0);
    tm t = *localtime(&now);
    char buf[11];
    strftime(buf, sizeof(buf), "%d/%m/%Y", &t);
    return string(buf);
}

void DiaCareManager::displayMenu() {
    cout << "\n========================================" << endl;
    cout << "        DiaCare Main Menu" << endl;
    cout << "========================================" << endl;
    cout << "1.  About Model" << endl;
    cout << "2.  Register New Patient" << endl;
    cout << "3.  Process Next Patient" << endl;
    cout << "4.  View Active Ward" << endl;
    cout << "5.  Navigate Ward" << endl;
    cout << "6.  New Checkup for Patient" << endl;
    cout << "7.  View Patient History" << endl;
    cout << "8.  Risk Trajectory Analysis" << endl;
    cout << "9.  Monitor Critical Patients" << endl;
    cout << "10. What-If Simulation" << endl;
    cout << "11. Discharge Patient" << endl;
    cout << "12. Undo Last Action" << endl;
    cout << "13. View Intake Queue" << endl;
    cout << "14. View Action History" << endl;
    cout << "15. Search Patient by ID" << endl;
    cout << "0.  Exit" << endl;
    cout << "========================================" << endl;
    cout << "  Choice: ";
}

void DiaCareManager::enterVitals(double features[TOTAL_RAW_FEATURES]) {

    string prompts[TOTAL_RAW_FEATURES] = {
        "Pregnancies              : ",
        "Glucose (mg/dL)          : ",
        "Blood Pressure (mmHg)    : ",
        "Skin Thickness (mm)      : ",
        "Insulin (mu U/ml)        : ",
        "BMI                      : ",
        "Diabetes Pedigree        : ",
        "Age                      : "
    };

    for (int i = 0; i < TOTAL_RAW_FEATURES; i++) {
        cout << prompts[i];
        cin  >> features[i];
    }
}



void DiaCareManager::admitToWard(Patient* patient) {
    activeWard.insertSorted(patient);
    patientIndex.insert(patient);

    if (Recommendation::isCritical(patient->probability)) {
        criticalMonitor.addPatient(patient);
    }
}

void DiaCareManager::removeFromWard(int id) {
    
    if (criticalMonitor.exists(id)) {
        criticalMonitor.removePatient(id);
    }
        

    // Remove from BST
    patientIndex.remove(id);

    // Remove from DLL + delete patient
    Patient* p = activeWard.removeById(id);
    if (p != nullptr) {
        delete p;
    }
        
}



void DiaCareManager::showBootScreen() {
    cout << "\n========================================" << endl;
    cout << "   DiaCare - Diabetes Advisory System" << endl;
    cout << "   Starting up..." << endl;
    cout << "========================================" << endl << endl;
}

void DiaCareManager::showReadyScreen() {
    cout << "\nSystem ready." << endl;
    cout << "========================================" << endl;
}



bool DiaCareManager::startup(bool debug) {

    showBootScreen();

    if (!pipeline.run(debug)) {
        cout << "Pipeline initialization failed." << endl;
        return false;
    }

    showReadyScreen();
    return true;
}


void DiaCareManager::registerPatient() {
    cout << "\n--- Register New Patient ---" << endl;

    string name;
    int age;

    cout << "Name: ";
    if (cin.peek() == '\n') {
        cin.ignore();
    }
    getline(cin, name);
    cout << "Age:  ";
    cin  >> age;

    Patient* patient = new Patient(nextPatientId++, name, age);

    intakeQueue.enqueue(patient);

    ActionRecord* record = new ActionRecord(ACTION_REGISTERED,patient->deepCopy());
    actionHistory.push(record);

    cout << "Registered successfully." << endl;
    cout << "Assigned ID: " << patient->id << endl;
}


void DiaCareManager::processNextPatient() {
    if (intakeQueue.isEmpty()) {
        cout << "\nNo patients in queue." << endl;
        return;
    }

    Patient* patient = intakeQueue.dequeue();

    cout << "\n--- Processing: " << patient->name << " ---" << endl;
    cout << "  Enter vitals:" << endl;

    double rawFeatures[TOTAL_RAW_FEATURES];
    enterVitals(rawFeatures);


    double prob = pipeline.predict_prob(rawFeatures);

    // Add checkup to SLL
    patient->addCheckup(rawFeatures,prob, getCurrentDate());

    patient->displayFull();


    admitToWard(patient);

    
    ActionRecord* record = new ActionRecord(ACTION_ADMITTED, patient->deepCopy() );
    actionHistory.push(record);

    cout << "\nPatient admitted to active ward." << endl;
}


void DiaCareManager::newCheckup() {
    cout << "\n--- New Checkup ---" << endl;
    cout << "Enter Patient ID: ";

    int id;
    cin >> id;

    Patient* patient = patientIndex.search(id);

    if (patient == nullptr) {
        cout << "Patient ID " << id << " not found." << endl;
        return;
    }

    // Remove from ward temporarily
    if (criticalMonitor.exists(id)) {
        criticalMonitor.removePatient(id);
    }
        

    patientIndex.remove(id);
    Patient* removed = activeWard.removeById(id);

    cout << "Patient: " << removed->name << endl;
    cout << "Enter new vitals:" << endl;

    double rawFeatures[TOTAL_RAW_FEATURES];
    enterVitals(rawFeatures);

   
    double prob = pipeline.predict_prob(rawFeatures);


    removed->addCheckup(rawFeatures, prob, getCurrentDate());


    removed->displayFull();

    admitToWard(removed);

    ActionRecord* record = new ActionRecord(ACTION_CHECKUP_ADDED, removed->deepCopy() );
    actionHistory.push(record);
}


void DiaCareManager::dischargePatient() {
    cout << "\n--- Discharge Patient ---" << endl;
    cout << "Enter Patient ID: ";

    int id;
    cin >> id;

    Patient* patient = patientIndex.search(id);

    if (patient == nullptr) {
        cout << "  Patient ID " << id << " not found.\n";
        return;
    }

    cout << "Discharging: " << patient->name << "    ID: " << patient->id << endl;

    // Deep copy for undo BEFORE deleting
    ActionRecord* record = new ActionRecord(ACTION_DISCHARGED, patient->deepCopy());
    actionHistory.push(record);

    removeFromWard(id);

    cout << "Patient discharged successfully." << endl;
}


void DiaCareManager::undoLastAction() {
    ActionRecord* record = actionHistory.peek();
    actionHistory.pop();
    if (record == nullptr) {
        return;
    }


    cout << "\n--- Undo Last Action ---" << endl;

    if (record->type == ACTION_ADMITTED || record->type == ACTION_CHECKUP_ADDED) {
        int id = record->snapshot->id;
        cout << "Reversing: " << record->snapshot->name << endl;

        removeFromWard(id);
        cout << "Patient removed from ward." << endl;
    } else if (record->type == ACTION_DISCHARGED) {
        cout << "Restoring: " << record->snapshot->name << endl;

        Patient* restored = record->snapshot;

        admitToWard(restored);
        record->snapshot = nullptr;

        cout << "Patient re-admitted." << endl;
    }
    else if (record->type == ACTION_REGISTERED) {
        cout << "Cannot undo registration." << endl;
    }

    delete record;
}


void DiaCareManager::whatIfSimulation() {
    cout << "\n--- What-If Simulation ---" << endl;
    cout << "Enter Patient ID: ";

    int id;
    cin >> id;


    Patient* patient = patientIndex.search(id);

    if (patient == nullptr) {
        cout << "  Patient ID " << id << " not found." << endl;
        return;
    }

    cout << "\nPatient: " << patient->name << endl;
    cout << "Current Risk: " << patient->riskLevel << " P: " << patient->probability * 100 << "%" << endl;

    cout << "\nEnter simulated vitals:" << endl;;

    double simFeatures[TOTAL_RAW_FEATURES];
    enterVitals(simFeatures);


   
    double simProb = pipeline.predict_prob(simFeatures);

    // Show result
    cout << "\n========================================" << endl;
    cout << "  What-If Simulation Result" << endl;;
    cout << "========================================" << endl;
    cout << "Patient : " << patient->name  << endl;
    cout << "BEFORE  : " << patient->riskLevel << "  P: " << patient->probability * 100 << "%" << endl;
    cout << "AFTER   : " << Recommendation::getRiskLevel(simProb) << "  P: " << simProb * 100 << "%" << endl;

    double diff = patient->probability - simProb;

    if (diff > 0) {
        cout << "\nAdvisory: These changes REDUCE " << "risk by " << diff * 100 << "%" << endl;
    } else if (diff < 0) {
        cout << "\nAdvisory: These changes INCREASE " << "risk by " << (-diff) * 100 << "%" << endl;
    } else {
        cout << "\nAdvisory: No change in risk." << endl;
    }

    cout << "========================================" << endl;
    cout << "  Real patient data unchanged." << endl;
}



void DiaCareManager::viewWard() {
    activeWard.display();
}



void DiaCareManager::navigateWard() {
    if (activeWard.isEmpty()) {
        cout << "\n  Active ward is empty." << endl;
        return;
    }

    activeWard.resetNavigation();
    Patient* current = activeWard.getCurrent();

    if (current == nullptr) {
        return;
    }

    char choice = ' ';
    while (1) {
        current->displayFull();

        cout << "\n[N] Next  [P] Previous  [Q] Quit" << endl;
        cout << "Choice: ";
        cin  >> choice;

        if (choice == 'n' || choice == 'N') {
            Patient* next = activeWard.goNext();
            if (next != nullptr) {
                current = next;
            }
        }

        else if (choice == 'p' || choice == 'P') {
            Patient* prev = activeWard.goPrev();
            if (prev != nullptr) {
                current = prev;
            }
        } 
        
        else if (choice == 'q' || choice == 'Q') {
            break;
        } 
        
        else {
            cout << "Invalid Input!" << endl;
        }
    }
}


void DiaCareManager::viewPatientHistory() {
    cout << "\n--- View Patient History ---" << endl;
    cout << "Enter Patient ID: ";

    int id;
    cin >> id;


    Patient* patient = patientIndex.search(id);

    if (patient == nullptr) {
        cout << "Patient ID " << id << " not found." << endl;
        return;
    }

    patient->displayHistory();
}



void DiaCareManager::viewRiskTrajectory() {
    cout << "\n--- Risk Trajectory Analysis ---" << endl;
    cout << "  Enter Patient ID: ";

    int id;
    cin >> id;

    Patient* patient = patientIndex.search(id);

    if (patient == nullptr) {
        cout << "Patient ID " << id << " not found." << endl;
        return;
    }

    double slope = patient->getRiskTrajectory();

    cout << "\nPatient : " << patient->name  << endl;
    cout << "Visits  : " << patient->visitCount << endl;
    cout << "Risk    : " << patient->riskLevel  << endl;
    cout << "P       : " << patient->probability * 100 << "%" << endl;
    cout << "Slope   : " << slope * 100 << "%  per visit" << endl;

    if (slope > 0){
        cout << "Trend   : INCREASING" << endl;
    }
        
    else if (slope < 0){
        cout << "Trend   : DECREASING" << endl;
    }
        
    else{
        cout << "Trend   : STABLE" << endl;
    }
        

    if (patient->isRapidDeterioration()) {
        cout << "\n!! WARNING: RAPID DETERIORATION DETECTED !!" << endl;
        cout << "Immediate attention advised." << endl;;
    }
}


void DiaCareManager::monitorCriticalPatients() {
    if (criticalMonitor.isEmpty()) {
        cout << "No critical patients." << endl;
        return;
    }

    cout << "\n--- Critical Patient Monitor ---" << endl;
    cout << "[N] Next Patient  [Q] Quit" << endl;;

    Patient* current = criticalMonitor.getCurrent();
    
    if (current != nullptr) {
        current->displaySummary();
    }
        
    char choice = ' ';
    while (1) {
        cout << "\nChoice: ";
        cin  >> choice;

        if (choice == 'n' || choice == 'N') {
            Patient* next = criticalMonitor.getNext();
            if (next != nullptr) {
                cout << endl;
                next->displaySummary();
            }
        } else if (choice == 'q' || choice == 'Q') {
            break;
        } else {
            cout << "Invalid Input!" << endl;
        }
    }
}


void DiaCareManager::viewQueue() {
    intakeQueue.display();
}


void DiaCareManager::viewActionHistory() {
    actionHistory.display();
}


void DiaCareManager::searchPatient() {
    cout << "\n--- Search Patient ---" << endl;
    cout << "  Enter Patient ID: ";

    int id;
    cin >> id;

    Patient* patient = patientIndex.search(id);

    if (patient == nullptr) {
        cout << "Patient ID " << id << " not found." << endl;
        return;
    }

    patient->displayFull();
}

void DiaCareManager::run() {
    
    int choice = -1;

    while (choice != 0) {
        displayMenu();
        
        cin >> choice;
        
        if (choice == 1) {
            pipeline.printModelReport();
        } else if (choice == 2) {
            registerPatient();
        }
        
        else if (choice == 3) {
            processNextPatient();
        }
        
        else if (choice == 4) {
            viewWard();
        }
        
        else if (choice == 5) {
            navigateWard();
        }
        
        else if (choice == 6) {
            newCheckup();
        }
        
        else if (choice == 7) {
            viewPatientHistory();
        }
        
        else if (choice == 8) {
            viewRiskTrajectory();
        }
        
        else if (choice == 9) {
            monitorCriticalPatients();
        }
        
        else if (choice == 10) {
            whatIfSimulation();
        }
        
        else if (choice == 11) {
            dischargePatient();
        }
        
        else if (choice == 12) {
            undoLastAction();
        }
        
        else if (choice == 13) {
            viewQueue();
        }
        
        else if (choice == 14) {
            viewActionHistory();
        }
        
        else if (choice == 15) {
            searchPatient();
        }
        
        else if (choice == 0) {
            cout << "\nGoodbye." << endl << endl;
        }
        
        else {
            cout << "Invalid choice." << endl;
        }
    }
}