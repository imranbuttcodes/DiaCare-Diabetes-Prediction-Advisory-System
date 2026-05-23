#pragma once

#include "config.h"
#include "patient.h"

struct StackNode {
    ActionRecord* record;
    StackNode* next;

    StackNode(ActionRecord* r);
};

// STACK:  Undo Mechanism + What-If Simulation

class Stack {
private:
    StackNode* top;
    int size;

public:
    Stack();
    ~Stack();

    
    void push(ActionRecord* record);
    void pop();
    ActionRecord* peek();

 
    bool isEmpty();
    int getSize();

    void display();
};