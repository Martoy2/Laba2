#include "avl_tree.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include "array.h"
#include "serialization.h"

static bool isNumber(const std::string& text) {
    std::size_t start = (!text.empty() && text[0] == '-') ? 1 : 0;
    if (start == text.size()) {
        return false;
    }

    for (std::size_t i = start; i < text.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(text[i]))) {
            return false;
        }
    }
    return true;
}

static int compareNumbers(const std::string& a, const std::string& b) {
    bool negativeA = a[0] == '-';
    bool negativeB = b[0] == '-';

    std::string digitsA = a.substr(negativeA ? 1 : 0);
    std::string digitsB = b.substr(negativeB ? 1 : 0);
    digitsA.erase(0, std::min(digitsA.find_first_not_of('0'), digitsA.size() - 1));
    digitsB.erase(0, std::min(digitsB.find_first_not_of('0'), digitsB.size() - 1));

    if (digitsA == "0") negativeA = false;
    if (digitsB == "0") negativeB = false;

    if (negativeA != negativeB) {
        return negativeA ? -1 : 1;
    }

    int result = 0;
    if (digitsA.size() != digitsB.size()) {
        result = digitsA.size() < digitsB.size() ? -1 : 1;
    } else {
        int cmp = digitsA.compare(digitsB);
        result = (cmp > 0) - (cmp < 0);
    }
    return negativeA ? -result : result;
}

AVLTree::AVLTree() : root(nullptr), sz(0) {}

AVLTree::AVLTree(std::initializer_list<std::string> init) : root(nullptr), sz(0) {
    for (const std::string& value : init) {
        insert(value);
    }
}

AVLTree::AVLTree(const AVLTree& other) : root(copy(other.root)), sz(other.sz) {}

AVLTree& AVLTree::operator=(const AVLTree& other) {
    if (this == &other) return *this;

    TreeNode* newRoot = copy(other.root);
    destroy(root);
    root = newRoot;
    sz = other.sz;
    return *this;
}

AVLTree::~AVLTree() {
    destroy(root);
}

int AVLTree::compare(const std::string& a, const std::string& b) {  // O(длины строки)
    bool numberA = isNumber(a);
    bool numberB = isNumber(b);

    if (numberA && numberB) {
        int result = compareNumbers(a, b);
        if (result != 0) {
            return result;
        }
    } else if (numberA != numberB) {
        return numberA ? -1 : 1;
    }

    int cmp = a.compare(b);
    return (cmp > 0) - (cmp < 0);
}

int AVLTree::heightOf(const TreeNode* node) {  // O(1)
    return node == nullptr ? 0 : node->height;
}

int AVLTree::balanceOf(const TreeNode* node) {  // O(1)
    return heightOf(node->left) - heightOf(node->right);
}

void AVLTree::updateHeight(TreeNode* node) {  // O(1)
    node->height = 1 + std::max(heightOf(node->left), heightOf(node->right));
}

TreeNode* AVLTree::rotateLeft(TreeNode* node) {  // O(1)
    TreeNode* right = node->right;
    node->right = right->left;
    right->left = node;

    updateHeight(node);
    updateHeight(right);
    return right;
}

TreeNode* AVLTree::rotateRight(TreeNode* node) {  // O(1)
    TreeNode* left = node->left;
    node->left = left->right;
    left->right = node;

    updateHeight(node);
    updateHeight(left);
    return left;
}

TreeNode* AVLTree::rebalance(TreeNode* node) {  // O(1) - максимум 2 поворота
    updateHeight(node);
    int balance = balanceOf(node);

    if (balance > 1) {
        if (balanceOf(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    if (balance < -1) {
        if (balanceOf(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    return node;
}

TreeNode* AVLTree::insert(TreeNode* node, const std::string& value, bool& inserted) {  // O(log n)
    if (node == nullptr) {
        inserted = true;
        return new TreeNode{value, nullptr, nullptr, 1};
    }

    int cmp = compare(value, node->value);
    if (cmp < 0) {
        node->left = insert(node->left, value, inserted);
    } else if (cmp > 0) {
        node->right = insert(node->right, value, inserted);
    } else {
        return node;
    }
    return rebalance(node);
}

TreeNode* AVLTree::remove(TreeNode* node, const std::string& value, bool& removed) {  // O(log n)
    if (node == nullptr) {
        return nullptr;
    }

    int cmp = compare(value, node->value);
    if (cmp < 0) {
        node->left = remove(node->left, value, removed);
    } else if (cmp > 0) {
        node->right = remove(node->right, value, removed);
    } else {
        removed = true;

        if (node->left == nullptr || node->right == nullptr) {
            TreeNode* child = node->left != nullptr ? node->left : node->right;
            delete node;
            return child;
        }

        TreeNode* successor = node->right;
        while (successor->left != nullptr) {
            successor = successor->left;
        }
        std::string successorValue = successor->value;
        bool successorRemoved = false;
        node->right = remove(node->right, successorValue, successorRemoved);
        node->value = successorValue;
    }
    return rebalance(node);
}

TreeNode* AVLTree::copy(const TreeNode* node) {
    if (node == nullptr) {
        return nullptr;
    }

    return new TreeNode{node->value, copy(node->left), copy(node->right), node->height};
}

void AVLTree::destroy(TreeNode* node) {
    if (node == nullptr) return;

    destroy(node->left);
    destroy(node->right);
    delete node;
}

bool AVLTree::insert(const std::string& value) {  // O(log n)
    bool inserted = false;
    root = insert(root, value, inserted);
    if (inserted) {
        ++sz;
    }
    return inserted;
}

bool AVLTree::remove(const std::string& value) {  // O(log n)
    bool removed = false;
    root = remove(root, value, removed);
    if (removed) {
        --sz;
    }
    return removed;
}

bool AVLTree::find(const std::string& value) const {  // O(log n)
    TreeNode* cur = root;
    while (cur != nullptr) {
        int cmp = compare(value, cur->value);
        if (cmp == 0) {
            return true;
        }
        cur = cmp < 0 ? cur->left : cur->right;
    }
    return false;
}

std::size_t AVLTree::size() const {  // O(1)
    return sz;
}

bool AVLTree::empty() const {  // O(1)
    return sz == 0;
}

int AVLTree::height() const {  // O(1)
    return heightOf(root);
}

void AVLTree::clear() {  // O(n)
    destroy(root);
    root = nullptr;
    sz = 0;
}

void AVLTree::write(std::ostream& os, const std::string& value, bool& first) {
    if (!first) {
        os << ", ";
    }
    os << value;
    first = false;
}

void AVLTree::inorder(std::ostream& os, const TreeNode* node, bool& first) {  // O(n)
    if (node == nullptr) return;

    inorder(os, node->left, first);
    write(os, node->value, first);
    inorder(os, node->right, first);
}

void AVLTree::preorder(std::ostream& os, const TreeNode* node, bool& first) {  // O(n)
    if (node == nullptr) return;

    write(os, node->value, first);
    preorder(os, node->left, first);
    preorder(os, node->right, first);
}

void AVLTree::postorder(std::ostream& os, const TreeNode* node, bool& first) {  // O(n)
    if (node == nullptr) return;

    postorder(os, node->left, first);
    postorder(os, node->right, first);
    write(os, node->value, first);
}

void AVLTree::printInorder(std::ostream& os) const {  // O(n)
    bool first = true;
    os << "[";
    inorder(os, root, first);
    os << "]";
}

void AVLTree::printPreorder(std::ostream& os) const {  // O(n)
    bool first = true;
    os << "[";
    preorder(os, root, first);
    os << "]";
}

void AVLTree::printPostorder(std::ostream& os) const {  // O(n)
    bool first = true;
    os << "[";
    postorder(os, root, first);
    os << "]";
}

void AVLTree::printLevelOrder(std::ostream& os) const {  // O(n), очередь до n узлов
    os << "[";
    if (root != nullptr) {
        const TreeNode** queue = new const TreeNode*[sz];
        std::size_t head = 0;
        std::size_t tail = 0;
        queue[tail++] = root;

        while (head < tail) {
            const TreeNode* node = queue[head++];
            if (head > 1) {
                os << ", ";
            }
            os << node->value;

            if (node->left != nullptr) {
                queue[tail++] = node->left;
            }
            if (node->right != nullptr) {
                queue[tail++] = node->right;
            }
        }
        delete[] queue;
    }
    os << "]";
}

char AVLTree::type() const {
    return 'T';
}

void AVLTree::preorder(std::string& line, const TreeNode* node) {  // O(n)
    if (node == nullptr) return;

    if (!line.empty()) {
        line += ' ';
    }
    line += escapeValue(node->value);
    preorder(line, node->left);
    preorder(line, node->right);
}

std::string AVLTree::serialize() const {  // O(n)
    std::string line;
    preorder(line, root);

    return line;
}

TreeNode* AVLTree::buildPreorder(const Array& values, std::size_t& index,
                                 const std::string* lower, const std::string* upper) {  // O(n)
    if (index >= values.size()) {
        return nullptr;
    }

    std::string value = values.get(index);
    if ((lower != nullptr && compare(value, *lower) <= 0) ||
        (upper != nullptr && compare(value, *upper) >= 0)) {
        return nullptr;
    }

    TreeNode* node = new TreeNode{value, nullptr, nullptr, 1};
    ++index;
    node->left = buildPreorder(values, index, lower, &node->value);
    node->right = buildPreorder(values, index, &node->value, upper);
    return node;
}

int AVLTree::fixHeights(TreeNode* node) {  // O(n)
    if (node == nullptr) {
        return 0;
    }

    int left = fixHeights(node->left);
    int right = fixHeights(node->right);
    if (left < 0 || right < 0 || left - right > 1 || right - left > 1) {
        return -1;
    }

    node->height = 1 + std::max(left, right);
    return node->height;
}

void AVLTree::deserialize(const std::string& line) {  // O(n), при битом файле O(n log n)
    clear();

    Array values;
    std::istringstream in(line);
    std::string token;
    while (in >> token) {
        values.pushBack(unescapeValue(token));
    }

    std::size_t index = 0;
    root = buildPreorder(values, index, nullptr, nullptr);
    sz = index;

    if (index != values.size() || fixHeights(root) < 0) {
        clear();
        for (std::size_t i = 0; i < values.size(); ++i) {
            insert(values.get(i));
        }
    }
}

void AVLTree::print(std::ostream& os) const {  // O(n)
    os << *this;
}

std::ostream& operator<<(std::ostream& os, const AVLTree& tree) {  // O(n)
    tree.printInorder(os);

    return os;
}
