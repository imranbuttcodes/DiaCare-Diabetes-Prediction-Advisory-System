#pragma once

#include "config.h"
#include "patient.h"



struct CLLNode {
    Patient*  patient;   
    CLLNode*  next;

    CLLNode(Patient* p);
};


// CIRCULAR LINKED LIST: Critical Monitor
// Only patients with P >= 0.60 added here
// ML-Weighted: higher risk = more nodes

class CircularLinkedList {
private:
    CLLNode* head;       
    CLLNode* tail; 
    CLLNode* current;     // current monitor position
    int size;        

    void insertNode(Patient* patient);

    void removeFromHead();
    void removeFromTail();
    bool removeOne(int id);

public:
    CircularLinkedList();
    ~CircularLinkedList();


    void addPatient(Patient* patient);
    void removePatient(int id);

    // Monitor Cycling 
    Patient* getNext();       
    Patient* getCurrent();    

    bool isEmpty() const;
    int getSize() const;
    bool exists(int id) const;

    
    void display();
};