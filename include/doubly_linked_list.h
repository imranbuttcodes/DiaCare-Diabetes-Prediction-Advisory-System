#pragma once

#include "config.h"
#include "patient.h"


struct DLLNode {
    Patient*  patient;    
    DLLNode*  prev;
    DLLNode*  next;

    DLLNode(Patient* p);
};


// DOUBLY LINKED LIST: Active Ward
// Sorted descending by probability
// Bounded by WARD_MAX_CAPACITY

class DoublyLinkedList {
private:
    DLLNode* head;        // highest risk patient
    DLLNode* tail;        // lowest risk patient
    DLLNode* current;     // for navigation
    int size;

    DLLNode* findById(int id);

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    
    bool insertSorted(Patient* patient);
    Patient* removeById(int id);
    Patient* removeLowestRisk();    // auto-discharge tail

    Patient* goNext();              
    Patient* goPrev();              
    Patient* getCurrent();          
    void     resetNavigation();     // reset to head

    
    bool isFull();
    bool isEmpty();
    int getSize();
    bool exists(int id);


    void display();
};