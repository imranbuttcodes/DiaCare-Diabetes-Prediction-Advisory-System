#pragma once

#include "config.h"
#include "patient.h"


struct BSTNode {
    Patient* patient;
    int id; 
    BSTNode* left;
    BSTNode* right;

    BSTNode(Patient* p): patient(p), id(p->id), left(nullptr), right(nullptr) {}

};

// BST — Patient Search Index
// Ordered by patient ID
// O(log n) search, insert, delete
// Does NOT own Patient objects
// DLL owns patients

class BST {
private:

    BSTNode* root;
    int size;


    BSTNode* insertHelper(BSTNode* root,  Patient* patient);

    BSTNode* removeHelper(BSTNode* root, int id);

    BSTNode* findMin(BSTNode* root);

    Patient* searchHelper(BSTNode* root, int id);

    void inorderHelper(BSTNode* root);

    void destroyHelper(BSTNode* root);

public:

    BST();
    ~BST();


    void insert(Patient* patient);
    void remove(int id);
    Patient* search(int id);

    void displayInorder();   

    bool isEmpty();
    int getSize();
    bool exists(int id);
};