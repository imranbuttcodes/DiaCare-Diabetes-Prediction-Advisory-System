#include "../include/queue.h"
#include <iostream>

using namespace std;


QueueNode::QueueNode(Patient* p): patient(p), next(nullptr) {}


Queue::Queue():front(nullptr), rear(nullptr), size(0) {}

Queue::~Queue() {
    // Deletes all nodes
    // it Does NOT delete Patient objects here
    // because Queue does not own the patients
    while (!isEmpty()) {
        QueueNode* temp = front;
        front = front->next;
        delete temp;
    }
    rear = nullptr;
}


void Queue::enqueue(Patient* patient) {
    QueueNode* newNode = new QueueNode(patient);

    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    size++;

    cout << "Patient " << patient->name<< "added to intake queue." << endl;
    cout << "  Position in queue: " << size << endl;
}



Patient* Queue::dequeue() {
    if (isEmpty()) {
        cout << "Queue is empty. " << "No patients waiting" << endl;
        return nullptr;
    }

    QueueNode* temp = front;
    Patient* patient = front->patient;

    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }

    delete temp;
    size--;
    return patient;
}


Patient* Queue::peek() {
    if (isEmpty()) {
        cout << "  Queue is empty." << endl;
        return nullptr;
    }
    return front->patient;
}


bool Queue::isEmpty() {
    return front == nullptr;
}

int Queue::getSize() {
    return size;
}


void Queue::display() {
    if (isEmpty()) {
        cout << "Intake Queue is empty." << endl;
        return;
    }

    cout << "\n========================================" << endl;
    cout << "Patient Intake Queue (" << size << " waiting)" << endl;
    cout << "========================================" << endl;

    QueueNode* temp = front;
    int pos  = 1;

    while (temp != nullptr) {
        cout << "[" << pos << "] " << "ID: "   << temp->patient->id << "  Name: " << temp->patient->name << "  Age: "  << temp->patient->age << endl;
        temp = temp->next;
        pos++;
    }
    cout << "========================================" << endl;
}