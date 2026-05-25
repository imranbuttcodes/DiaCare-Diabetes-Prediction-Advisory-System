#include "../include/doubly_linked_list.h"
#include <iostream>
#include <iomanip>
using namespace std;



DLLNode::DLLNode(Patient* p): patient(p), prev(nullptr), next(nullptr) {}


DoublyLinkedList::DoublyLinkedList(): head(nullptr), tail(nullptr),current(nullptr), size(0) {}

DoublyLinkedList::~DoublyLinkedList() {
    // Delete all DLL nodes
    // it Does NOT delete Patient objects
    // HospitalManager owns patients
    DLLNode* temp = head;
    while (temp != nullptr) {
        DLLNode* next = temp->next;
        delete temp;
        temp = next;
    }
    head = tail = current = nullptr;
}


DLLNode* DoublyLinkedList::findById(int id) {
    DLLNode* temp = head;
    while (temp != nullptr) {
        if (temp->patient->id == id) {
            return temp;
        }

        temp = temp->next;
    }
    return nullptr;
}

// INSERT SORTED: Descending by probability

bool DoublyLinkedList::insertSorted(Patient* patient) {

    DLLNode* newNode = new DLLNode(patient);

    if (isEmpty()) {
        head = tail = current = newNode;
        size++;
        return true;
    }

    // Insert at head (highest prob) 
    if (patient->probability >= head->patient->probability) {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
        size++;
        return true;
    }

    // Insert at tail (lowest prob) 
    if (patient->probability <= tail->patient->probability) {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
        size++;
        return true;
    }

    // Insert in middle 
    DLLNode* temp = head->next;
    while (temp != nullptr) {
        if (patient->probability >= temp->patient->probability) {
            // Insert before temp
            newNode->next = temp;
            newNode->prev = temp->prev;
            temp->prev->next = newNode;
            temp->prev = newNode;
            size++;
            return true;
        }
        temp = temp->next;
    }

    return false;
}


Patient* DoublyLinkedList::removeById(int id) {
    DLLNode* node = findById(id);

    if (node == nullptr) {
        cout << "Patient ID " << id << " not found in ward" << endl;
        return nullptr;
    }

    Patient* patient = node->patient;

    // Fix navigation pointer
    if (current == node) {
        current = node->next ? node->next : node->prev;
    }
        

    // Fix head
    if (node == head) {
        head = node->next; 
        if (head != nullptr) {
            head->prev = nullptr;
        }
    }
    // Fix tail
    else if (node == tail) {
        tail = node->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        } 
    }
    // Middle node
    else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    delete node;
    size--;

    if (isEmpty()) {
        head = tail = current = nullptr;
    }
        
    return patient;
}

// REMOVE LOWEST RISK: Auto-discharge tail

Patient* DoublyLinkedList::removeLowestRisk() {
    if (isEmpty()) return nullptr;
    return removeById(tail->patient->id);
}


Patient* DoublyLinkedList::goNext() {
    if (current == nullptr) {
        cout << "  Ward is empty." << endl;
        return nullptr;
    }
    if (current->next == nullptr) {
        cout << "Already at lowest risk patient in ward." << endl;
        return current->patient;
    }
    current = current->next;
    return current->patient;
}

Patient* DoublyLinkedList::goPrev() {
    if (current == nullptr) {
        cout << "  Ward is empty" << endl;
        return nullptr;
    }
    
    if (current->prev == nullptr) {
        cout << "Already at highest risk patient in ward" << endl;
        return current->patient;
    }

    current = current->prev;
    return current->patient;
}

Patient* DoublyLinkedList::getCurrent() {
    if (current == nullptr) {
        return nullptr;
    } 

    return current->patient;
}

void DoublyLinkedList::resetNavigation() {
    current = head;
}


bool DoublyLinkedList::isEmpty() {
    return head == nullptr;
}

int DoublyLinkedList::getSize() {
    return size;
}

bool DoublyLinkedList::exists(int id) {
    return findById(id) != nullptr;
}

void DoublyLinkedList::display() {
    if (isEmpty()) {
        cout << "\n  Active Ward is empty.\n";
        return;
    }

    cout << "\n============================================================" << endl;;
    cout << "Active Ward (" << size << " patients)";
    cout << " Firsts = Highest Risk  |  last = Lowest Risk" << endl;
    cout << "============================================================" << endl;

    DLLNode* temp = head;
    int pos  = 1;

    while (temp != nullptr) {

        // Mark current navigation position
        string marker = (temp == current) ? "<-- YOU ARE HERE" : "";

        cout << "[" << pos << "] "
             << "ID: "   << temp->patient->id
             << " Name: " << temp->patient->name
             << " Age: "  << temp->patient->age
             << " Risk: " << temp->patient->riskLevel
             << " P: "
             << temp->patient->probability * 100
             << "% "
             << marker << endl;

        temp = temp->next;
        pos++;
    }

    cout << "========================================\n";
}