#include "../include/circular_linked_list.h"
#include <iostream>
using namespace std;


CLLNode::CLLNode(Patient* p): patient(p), next(nullptr) {}


CircularLinkedList::CircularLinkedList(): head(nullptr), tail(nullptr), current(nullptr), size(0) {}

CircularLinkedList::~CircularLinkedList() {
    if (isEmpty()) {
        return;
    }

    // Break the circle first
    tail->next = nullptr;
        
    // Now delete all nodes normally
    CLLNode* temp = head;
    while (temp != nullptr) {
        CLLNode* next = temp->next;
        delete temp;
        temp = next;
    }

    head = tail = current = nullptr;
}




void CircularLinkedList::removeFromHead() {
    if (isEmpty()) {
        return;
    }

    if (head->next == head) {
        delete head;
        head = current = tail =  nullptr;
        size--;
        return;
    }



    CLLNode* nextNode = head->next;
    
    if (current == head) {
        current = nextNode;    
    }

    delete head;
    
    head = nextNode;
    tail->next = head;
    
    size--;

}



void CircularLinkedList::removeFromTail() {

    if (isEmpty()) {
        return;
    }

    if (head == tail) {

        delete head;

        head = tail =  current = nullptr; 

        size--;

        return;
    }

   
    CLLNode* temp = head;

    while(temp->next != tail) {
        temp = temp->next;
    }
    
    if (current == tail) {
        current = head;
    }

    delete tail;

    tail = temp;

    tail->next = head;
    size--;
}



bool CircularLinkedList::removeOne(int id) {

    if (isEmpty()) {
        return false;
    }

    if (head->patient->id == id) {
        removeFromHead();
        return true;
    }

    if (tail->patient->id == id) {
        removeFromTail();
        return true;
    }

    CLLNode* prev = head;
    CLLNode* temp = head->next;

    while (temp != head) {

        if (temp->patient->id == id) {

            if (current == temp) {
                current = temp->next;
            }

            prev->next = temp->next;
            delete temp;
            size--;

            return true;
        }

        prev = temp;
        temp = temp->next;
    }

    return false;
}



void CircularLinkedList::insertNode(Patient* patient) {
    CLLNode* newNode = new CLLNode(patient);
    if (isEmpty()) {
        head = tail =  newNode;
        tail->next = head;    
        current = head;
        size++;
        return;
    }

    newNode->next = head;
    tail->next = newNode;
    tail = newNode;
          
    size++;
}


// ADD PATIENT: ML Weighted insertion
// P 0.60-0.75 -> 1 node
// P 0.76-0.89 -> 2 nodes
// P >= 0.90   -> 3 nodes

void CircularLinkedList::addPatient(Patient* patient) {
    if (!Recommendation::isCritical(patient->probability)) {
        cout << "Patient risk is below critical threshold. Not added to monitor.";
        return;
    }

    int weight = Recommendation::getCLLWeight(patient->probability);

    for (int i = 0; i < weight; i++) {
        insertNode(patient);
    }
        

    cout << "Patient " << patient->name << " added to critical monitor (" << weight << " node";
    if (weight > 1) {
        cout << "s";
    } 
    cout << " , Probablty: " << patient->probability * 100 << "%)." << endl;
}



void CircularLinkedList::removePatient(int id) {
   
    int removed = 0;
    
    while(removeOne(id)) {
        removed++;
    }

    if (removed > 0) {
        cout << "Patient ID " << id << " removed from monitor (" << removed << " node";
        if (removed > 1) {
            cout << "s";
        } 
        cout << " removed)." << endl;
    } else {
        cout << "Patient ID " << id << " not found in monitor." << endl;
    }
}


Patient* CircularLinkedList::getNext() {
    if (isEmpty()) {
        cout << "No critical patients to monitor." << endl;
        return nullptr;
    }

    if (current == nullptr) {
        current = head;
    }
        
    current = current->next;
    return current->patient;
}



Patient* CircularLinkedList::getCurrent() {
    if (isEmpty()) 
    {
        return nullptr;
    }
    return current->patient;
}


bool CircularLinkedList::isEmpty() const{
    return head == nullptr;
}

int CircularLinkedList::getSize() const{
    return size;
}

bool CircularLinkedList::exists(int id) const{
    if (isEmpty()) {
        return false;
    }

    CLLNode* temp = head;
    do {
        if (temp->patient->id == id) {
            return true;
        }
        temp = temp->next;
    } while (temp != head);

    return false;
}


void CircularLinkedList::display() {
    if (isEmpty()) {
        cout << "\n  No critical patients in monitor.\n";
        return;
    }

    cout << "\n========================================" << endl;
    cout << "Critical Patient Monitor" << endl;
    cout << "(" << size << " monitor slots)" << endl;
    cout << "Higher risk = appears more frequently" << endl;;
    cout << "========================================" << endl;

    CLLNode* temp = head;
    int pos = 1;

    do {
        string marker = (temp == current) ? "<-- CURRENT" : "";
        cout << "[" << pos << "] "
             << "ID: " << temp->patient->id
             << " Name: " << temp->patient->name
             << " Risk: " << temp->patient->riskLevel
             << " P: "
             << temp->patient->probability * 100
             << "%" << marker << endl;

        temp = temp->next;
        pos++;

    } while (temp != head);

    cout << "========================================\n";
}