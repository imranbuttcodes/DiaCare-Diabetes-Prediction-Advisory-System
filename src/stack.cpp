#include "../include/stack.h"
#include <iostream>
using namespace std;


StackNode::StackNode(ActionRecord* r): record(r), next(nullptr) {}


Stack::Stack() : top(nullptr), size(0) {}

Stack::~Stack() {
    
    while (!isEmpty()) {
        StackNode* temp = top;
        ActionRecord* record = top->record;
        top = top->next;
        delete record;                            
        
        delete temp;
        size--;
    }
}

void Stack::push(ActionRecord* record) {
    StackNode* newNode = new StackNode(record);
    newNode->next = top;
    top = newNode;
    size++;
}


void Stack::pop() {
    if (isEmpty()) {
        cout << "Nothing to undo." << endl;
        return;
    }

    StackNode* temp = top;

    top = top->next;
    delete temp;
    size--;

}


ActionRecord* Stack::peek() {
    if (isEmpty()) {
        cout << "Action history is empty";
        return nullptr;
    }
    return top->record;
}


bool Stack::isEmpty() {
    return top == nullptr;
}

int Stack::getSize() {
    return size;
}


void Stack::display() {
    if (isEmpty()) {
        cout << "\nAction history is empty." << endl;
        return;
    }

    cout << "\n========================================" << endl;
    cout << "Action History (most recent first)" << endl;
    cout << "Total actions: " << size << endl;
    cout << "========================================" << endl;

    StackNode* temp = top;
    int pos  = 1;

    while (temp != nullptr) {
        string actionName;
        if (temp->record->type == ACTION_REGISTERED) {
            actionName = "REGISTERED";
        }
            
        else if (temp->record->type == ACTION_ADMITTED) {
            actionName = "ADMITTED";
        }
            
        else if (temp->record->type == ACTION_DISCHARGED) {
            actionName = "DISCHARGED";
        }
        else if (temp->record->type == ACTION_CHECKUP_ADDED) {
            actionName = "CHECKUP ADDED";
        }
        else {
            actionName = "UNKNOWN";
        }

        cout << "  [" << pos << "] " << actionName << " Patient: "<< temp->record->snapshot->name<< "  ID: "<< temp->record->snapshot->id<< endl;

        temp = temp->next;
        pos++;
    }

    cout << "========================================" << endl;
}