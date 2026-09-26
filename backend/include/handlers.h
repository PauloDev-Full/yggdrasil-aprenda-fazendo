#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdbool.h>

typedef struct Node {
    int value;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

Node* insert(Node* root, int value);
Node* deleteNode(Node* root, int value);
bool functionBalance(Node* root);
Node* rotateInSpecificValue(Node* root, int value, int typeRotation);

#endif
