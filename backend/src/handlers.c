#include "../include/handlers.h"
#include <stdio.h>
#include <stdlib.h>

Node* initializerNode(int value){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o nó.\n");
        exit(EXIT_FAILURE);
    }
    newNode->value = value;
    newNode->height = 0;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

static int obterAltura(Node* node) {
    if (node == NULL) {
        return -1;
    }
    return node->height;
}

static Node* lowerNode(Node* node){
    Node* lower = node;
    while(lower->left != NULL){
        lower = lower->left;
    }

    return lower;
}

static int max(int x, int y) {
    return (x > y) ? x : y;
}

bool functionBalance(Node* root) {
    if (root == NULL) {
        return true;
    }
    Node* leftChild = (root)->left;
    Node* rightChild = (root)->right;

    if (leftChild == NULL && rightChild == NULL) {
        return true;
    }
    else if (leftChild == NULL) {
        if (rightChild->height > 0) {
           return false;
        }
    } else if (rightChild == NULL) {
        if (leftChild->height > 0) {
            return false;
        }
    }
    else if (leftChild->height - rightChild->height > 1) {
        return false;
    } else if (rightChild->height - leftChild->height > 1) {
        return false;
    }
    return true;
}

Node* insert(Node* root, int value) {
    if (root == NULL) {
        return initializerNode(value);
    } else if (value < root->value) {
        root->left = insert(root->left, value);
    } else if (value > root->value) {
        root->right = insert(root->right, value);
    }
    
    root->height = 1 + max(obterAltura(root->left), obterAltura(root->right));

    if (!functionBalance(root)) {
        printf("[AVISO] O no %d acabou de sofrer um desbalanceamento!\n", (root)->value);
    }
    return root;
}

Node* deleteNode(Node* root, int value){
    if (root == NULL) {
        return NULL;
    }
    else if(value == root->value){
        Node* temp1 = root;
        if((temp1->left != NULL) && (temp1->right != NULL)){
            Node* temp2 = lowerNode(temp1->right);
            root->value = temp2->value;
            root->right = deleteNode(root->right, temp2->value);
        }
        else if((temp1->left != NULL) && (temp1->right == NULL)){
            root = temp1->left;
            free(temp1);
            return root;
        }
        else if((temp1->left == NULL) && (temp1->right != NULL)){
            root = temp1->right;
            free(temp1);
            return root;
        }
        else{
            free(temp1);
            return NULL;
        }
    }
    else if (value < root->value){
        root->left = deleteNode(root->left, value);
    }
    else if (value > root->value){
        root->right = deleteNode(root->right, value);
    }

    if (root == NULL) {
        return NULL;
    }
    
    root->height = 1 + max(obterAltura(root->left), obterAltura(root->right));
    
    if (!functionBalance(root)) {
        printf("[AVISO] O no %d acabou de sofrer um desbalanceamento apos a remocao!\n", root->value);
    }
    return root;
}

static Node* rotateLeft(Node* root) {
    Node* temp = root;
    Node* node = temp->right;
    temp->right = node->left;
    node->left = temp;
    temp->height = 1 + max(obterAltura(temp->left), obterAltura(temp->right));
    node->height = 1 + max(obterAltura(node->left), obterAltura(node->right));
    return node;
}

static Node* rotateRight(Node* root) {
    Node* temp = root;
    Node* node = temp->left;
    temp->left = node->right;
    node->right = temp;
    temp->height = 1 + max(obterAltura(temp->left), obterAltura(temp->right));
    node->height = 1 + max(obterAltura(node->left), obterAltura(node->right));
    return node;
}

Node* rotateInSpecificValue(Node* root, int value, int typeRotation){
    if(root == NULL){
        return NULL;
    }
    else if(root->value > value){
        root->left = rotateInSpecificValue(root->left, value, typeRotation);
    }
    else if(root->value < value){
        root->right = rotateInSpecificValue(root->right, value, typeRotation);
    }
        else if(root->value == value){
            if(typeRotation == 1) {
                return rotateRight(root);
            } else if (typeRotation == 2) {
                return rotateLeft(root);
            }
    }
    return root;
}

Node* resetTree(Node* root){
    if(root == NULL){
        return NULL;
    }
    root->left = resetTree(root->left);
    root->right = resetTree(root->right);
    free(root);
    return NULL;
}