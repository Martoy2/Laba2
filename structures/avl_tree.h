#ifndef INC_2C_LABA1_AVL_TREE_H
#define INC_2C_LABA1_AVL_TREE_H
#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <string>
#include "node.h"
#include "container.h"

class Array;

class AVLTree : public Container {
public:
    AVLTree();
    AVLTree(std::initializer_list<std::string> init);

    AVLTree(const AVLTree& other);
    AVLTree& operator=(const AVLTree& other);

    ~AVLTree();

public:
    bool insert(const std::string& value);
    bool remove(const std::string& value);
    bool find(const std::string& value) const;

    std::size_t size() const;
    bool empty() const;
    int height() const;
    void clear();

    void printInorder(std::ostream& os) const;
    void printPreorder(std::ostream& os) const;
    void printPostorder(std::ostream& os) const;
    void printLevelOrder(std::ostream& os) const;

public:
    char type() const override;
    std::string serialize() const override;
    void deserialize(const std::string& line) override;
    void print(std::ostream& os) const override;

public:
    friend std::ostream& operator<<(std::ostream& os, const AVLTree& tree);

private:
    static int compare(const std::string& a, const std::string& b);

    static int heightOf(const TreeNode* node);
    static int balanceOf(const TreeNode* node);
    static void updateHeight(TreeNode* node);
    static TreeNode* rotateLeft(TreeNode* node);
    static TreeNode* rotateRight(TreeNode* node);
    static TreeNode* rebalance(TreeNode* node);

    static TreeNode* insert(TreeNode* node, const std::string& value, bool& inserted);
    static TreeNode* remove(TreeNode* node, const std::string& value, bool& removed);

    static TreeNode* copy(const TreeNode* node);
    static void destroy(TreeNode* node);

    static TreeNode* buildPreorder(const Array& values, std::size_t& index,
                                   const std::string* lower, const std::string* upper);
    static int fixHeights(TreeNode* node);

    static void write(std::ostream& os, const std::string& value, bool& first);
    static void inorder(std::ostream& os, const TreeNode* node, bool& first);
    static void preorder(std::ostream& os, const TreeNode* node, bool& first);
    static void postorder(std::ostream& os, const TreeNode* node, bool& first);
    static void preorder(std::string& line, const TreeNode* node);

private:
    TreeNode* root;

    std::size_t sz;
};

#endif
