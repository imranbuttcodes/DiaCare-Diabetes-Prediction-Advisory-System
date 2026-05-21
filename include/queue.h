#pragma once

#include "config.h"
#include "patient.h"

struct QueueNode {
    Patient* patient;    
    QueueNode* next;

    QueueNode(Patient* p);
};


class Queue {
private:
    QueueNode* front;      
    QueueNode* rear;       
    int size;

public:
    Queue();
    ~Queue();

    void enqueue(Patient* patient);
    Patient* dequeue();
    Patient* peek();


    bool isEmpty();
    int getSize();
    void display();
};