#ifndef INC_2C_LABA1_NODE_H
#define INC_2C_LABA1_NODE_H
#include <string>

struct Node {
    std::string value;
    Node* next;
};

struct DNode {
    std::string value;
    DNode* prev;
    DNode* next;
};

struct TreeNode {
    std::string value;
    TreeNode* left;
    TreeNode* right;
    int height;
};

#endif
