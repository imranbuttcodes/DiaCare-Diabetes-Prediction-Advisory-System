#include "../include/BST.h"

BST::BST(): root(nullptr), size(0) {}


BSTNode* BST::insertHelper(BSTNode* root,  Patient* patient) {
    if (root == nullptr) {
        return new BSTNode(patient);
    }

    if (patient->id > root->id) {
        root->right = insertHelper(root->right, patient);
    } else if (patient->id < root->id){
        root->left = insertHelper(root->left, patient);
    }

    return root;
} 






BSTNode* BST::removeHelper(BSTNode* root, int id) {
    if (root == nullptr) {
        return nullptr;
    }

    if (id < root->id) {
        root->left = removeHelper(root->left, id);

    } else if (id > root->id) {
        root->right = removeHelper(root->right, id);

    } else {

        if (root->left != nullptr && root->right != nullptr) {
            
            BSTNode* inorder_successor = findMin(root->right);
            
            root->patient = inorder_successor->patient;
            
            root->id = inorder_successor->id;
            
            root->right = removeHelper(root->right, inorder_successor->id);

        } else if (root->left != nullptr) {
            BSTNode* toReturn = root->left;
            delete root;    

            return toReturn;

        } else if (root->right != nullptr) {
            BSTNode* toReturn = root->right;
            delete root;

            return toReturn;

        } else {
            delete root;
            return nullptr;
           
        }



    }

    return root;
}



BSTNode* BST::findMin(BSTNode* root) {


    if (root == nullptr || root->left == nullptr) {
        return root;
    }

    return findMin(root->left);
}


Patient* BST::searchHelper(BSTNode* root, int id) {

    if (root == nullptr) {
        return nullptr;
    }

    if (root->id == id) {
        return root->patient;
    }

    if (id < root->id) {
        return searchHelper(root->left, id);
    } else {
        return searchHelper(root->right, id);
    }



}

void BST::inorderHelper(BSTNode* root) {
    if (root == nullptr) {
        return;
    }

    inorderHelper(root->left);

    cout << "ID: " << root->id << endl;

    inorderHelper(root->right);

}

void BST::destroyHelper(BSTNode* root) {
    if (root == nullptr) {
        return;
    }

    destroyHelper(root->left);
    destroyHelper(root->right);

    delete root;

}


BST::~BST() {
    destroyHelper(root);
}


void BST::insert(Patient* patient) {
    root = insertHelper(root, patient);
        size++;
    
}

void BST::remove(int id) {
    if (exists(id)) {
        root = removeHelper(root, id);
        size--;
    } else {
        cout << "Patient with id " << id <<  " Not found!" << endl;
    }

}


Patient* BST::search(int id) {
    return searchHelper(root, id);
}

void BST::displayInorder() {
    if (root == nullptr) {
        cout << "EMPTY! No Patient added!" << endl;
        return;
    }
    inorderHelper(root);
}

bool BST::isEmpty() {
    return size == 0;
}

int BST::getSize() {
    return size;
}

bool BST::exists(int id) {
    return searchHelper(root, id) != nullptr;
}