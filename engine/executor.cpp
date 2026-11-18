#include "executor.h"
#include <cctype>
#include <sstream>
#include <stdexcept>
#include "../structures/forward_list.h"
#include "../structures/doubly_list.h"
#include "../structures/stack.h"
#include "../structures/queue.h"
#include "../structures/avl_tree.h"
#include "../structures/hash_set.h"

static std::string toUpper(const std::string& text) {
    std::string result = text;
    for (char& c : result) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return result;
}

static void requireArgs(const Array& tokens, std::size_t count) {
    if (tokens.size() != count) {
        throw std::invalid_argument("wrong number of arguments for " + tokens.get(0));
    }
}

static std::size_t parseIndex(const std::string& token) {
    if (token.empty() || token.size() > 18) {
        throw std::invalid_argument("bad index: " + token);
    }

    std::size_t index = 0;
    for (char c : token) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("bad index: " + token);
        }
        index = index * 10 + static_cast<std::size_t>(c - '0');
    }
    return index;
}

static void checkName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("empty structure name");
    }
    for (char c : name) {
        if (std::isspace(static_cast<unsigned char>(c)) || c == '\\') {
            throw std::invalid_argument("bad structure name: " + name);
        }
    }
}

template <typename T>
static T* findAs(Storage& storage, const std::string& name, char type, bool create) {
    Container* container = storage.find(name);
    if (container == nullptr) {
        if (!create) {
            throw std::invalid_argument("structure not found: " + name);
        }
        checkName(name);
        container = Storage::create(type);
        storage.add(name, container);
    }

    T* result = dynamic_cast<T*>(container);
    if (result == nullptr) {
        throw std::invalid_argument(name + " is not of type " + std::string(1, type));
    }
    return result;
}

static std::string show(const Container& container) {
    std::ostringstream out;
    container.print(out);
    return out.str();
}

static std::string executeArray(Storage& storage, const std::string& op, const Array& tokens, bool& changed) {
    std::string name = tokens.get(1);

    if (op == "PUSH") {
        requireArgs(tokens, 3);
        findAs<Array>(storage, name, 'M', true)->pushBack(tokens.get(2));
        changed = true;
        return tokens.get(2);
    }
    if (op == "INSERT") {
        requireArgs(tokens, 4);
        std::size_t index = parseIndex(tokens.get(2));
        findAs<Array>(storage, name, 'M', true)->insert(index, tokens.get(3));
        changed = true;
        return tokens.get(3);
    }
    if (op == "GET") {
        requireArgs(tokens, 3);
        return findAs<Array>(storage, name, 'M', false)->get(parseIndex(tokens.get(2)));
    }
    if (op == "DEL") {
        requireArgs(tokens, 3);
        Array* array = findAs<Array>(storage, name, 'M', false);
        std::size_t index = parseIndex(tokens.get(2));
        std::string value = array->get(index);
        array->remove(index);
        changed = true;
        return value;
    }
    if (op == "SET") {
        requireArgs(tokens, 4);
        findAs<Array>(storage, name, 'M', false)->set(parseIndex(tokens.get(2)), tokens.get(3));
        changed = true;
        return tokens.get(3);
    }
    if (op == "LEN") {
        requireArgs(tokens, 2);
        return std::to_string(findAs<Array>(storage, name, 'M', false)->size());
    }

    throw std::invalid_argument("unknown command: " + tokens.get(0));
}

template <typename List>
static std::string executeList(Storage& storage, char type, const std::string& op, const Array& tokens, bool& changed) {
    std::string name = tokens.get(1);

    if (op == "PUSH" || op == "PUSHTAIL") {
        requireArgs(tokens, 3);
        findAs<List>(storage, name, type, true)->pushBack(tokens.get(2));
        changed = true;
        return tokens.get(2);
    }
    if (op == "PUSHHEAD") {
        requireArgs(tokens, 3);
        findAs<List>(storage, name, type, true)->pushFront(tokens.get(2));
        changed = true;
        return tokens.get(2);
    }
    if (op == "PUSHBEFORE") {
        requireArgs(tokens, 4);
        findAs<List>(storage, name, type, false)->insertBefore(tokens.get(2), tokens.get(3));
        changed = true;
        return tokens.get(3);
    }
    if (op == "PUSHAFTER") {
        requireArgs(tokens, 4);
        findAs<List>(storage, name, type, false)->insertAfter(tokens.get(2), tokens.get(3));
        changed = true;
        return tokens.get(3);
    }
    if (op == "DELHEAD") {
        requireArgs(tokens, 2);
        List* list = findAs<List>(storage, name, type, false);
        std::string value = list->front();
        list->popFront();
        changed = true;
        return value;
    }
    if (op == "DELTAIL") {
        requireArgs(tokens, 2);
        List* list = findAs<List>(storage, name, type, false);
        std::string value = list->back();
        list->popBack();
        changed = true;
        return value;
    }
    if (op == "DELBEFORE") {
        requireArgs(tokens, 3);
        findAs<List>(storage, name, type, false)->removeBefore(tokens.get(2));
        changed = true;
        return "OK";
    }
    if (op == "DELAFTER") {
        requireArgs(tokens, 3);
        findAs<List>(storage, name, type, false)->removeAfter(tokens.get(2));
        changed = true;
        return "OK";
    }
    if (op == "DEL") {
        requireArgs(tokens, 3);
        findAs<List>(storage, name, type, false)->removeValue(tokens.get(2));
        changed = true;
        return tokens.get(2);
    }
    if (op == "FIND") {
        requireArgs(tokens, 3);
        return findAs<List>(storage, name, type, false)->find(tokens.get(2)) ? "TRUE" : "FALSE";
    }
    if (op == "GET") {
        requireArgs(tokens, 2);
        return show(*findAs<List>(storage, name, type, false));
    }
    if (op == "GETREV") {
        requireArgs(tokens, 2);
        std::ostringstream out;
        findAs<List>(storage, name, type, false)->printReverse(out);
        return out.str();
    }
    if (op == "LEN") {
        requireArgs(tokens, 2);
        return std::to_string(findAs<List>(storage, name, type, false)->size());
    }

    throw std::invalid_argument("unknown command: " + tokens.get(0));
}

static std::string executeStack(Storage& storage, const std::string& op, const Array& tokens, bool& changed) {
    std::string name = tokens.get(1);

    if (op == "PUSH") {
        requireArgs(tokens, 3);
        findAs<Stack>(storage, name, 'S', true)->push(tokens.get(2));
        changed = true;
        return tokens.get(2);
    }
    if (op == "POP") {
        requireArgs(tokens, 2);
        std::string value = findAs<Stack>(storage, name, 'S', false)->pop();
        changed = true;
        return value;
    }
    if (op == "PEEK") {
        requireArgs(tokens, 2);
        return findAs<Stack>(storage, name, 'S', false)->top();
    }
    if (op == "LEN") {
        requireArgs(tokens, 2);
        return std::to_string(findAs<Stack>(storage, name, 'S', false)->size());
    }

    throw std::invalid_argument("unknown command: " + tokens.get(0));
}

static std::string executeQueue(Storage& storage, const std::string& op, const Array& tokens, bool& changed) {
    std::string name = tokens.get(1);

    if (op == "PUSH") {
        requireArgs(tokens, 3);
        findAs<Queue>(storage, name, 'Q', true)->push(tokens.get(2));
        changed = true;
        return tokens.get(2);
    }
    if (op == "POP") {
        requireArgs(tokens, 2);
        std::string value = findAs<Queue>(storage, name, 'Q', false)->pop();
        changed = true;
        return value;
    }
    if (op == "PEEK") {
        requireArgs(tokens, 2);
        return findAs<Queue>(storage, name, 'Q', false)->front();
    }
    if (op == "LEN") {
        requireArgs(tokens, 2);
        return std::to_string(findAs<Queue>(storage, name, 'Q', false)->size());
    }

    throw std::invalid_argument("unknown command: " + tokens.get(0));
}

static std::string executeTree(Storage& storage, const std::string& op, const Array& tokens, bool& changed) {
    std::string name = tokens.get(1);

    if (op == "INSERT") {
        requireArgs(tokens, 3);
        if (!findAs<AVLTree>(storage, name, 'T', true)->insert(tokens.get(2))) {
            throw std::invalid_argument("value already in tree: " + tokens.get(2));
        }
        changed = true;
        return tokens.get(2);
    }
    if (op == "DEL") {
        requireArgs(tokens, 3);
        if (!findAs<AVLTree>(storage, name, 'T', false)->remove(tokens.get(2))) {
            throw std::invalid_argument("value not found: " + tokens.get(2));
        }
        changed = true;
        return tokens.get(2);
    }
    if (op == "FIND") {
        requireArgs(tokens, 3);
        return findAs<AVLTree>(storage, name, 'T', false)->find(tokens.get(2)) ? "TRUE" : "FALSE";
    }
    if (op == "GET") {
        if (tokens.size() != 2 && tokens.size() != 3) {
            throw std::invalid_argument("wrong number of arguments for " + tokens.get(0));
        }
        AVLTree* tree = findAs<AVLTree>(storage, name, 'T', false);
        std::string order = tokens.size() == 3 ? toUpper(tokens.get(2)) : "INORDER";
        std::ostringstream out;
        if (order == "INORDER") {
            tree->printInorder(out);
        } else if (order == "PREORDER") {
            tree->printPreorder(out);
        } else if (order == "POSTORDER") {
            tree->printPostorder(out);
        } else if (order == "BFS") {
            tree->printLevelOrder(out);
        } else {
            throw std::invalid_argument("unknown order: " + tokens.get(2));
        }
        return out.str();
    }
    if (op == "HEIGHT") {
        requireArgs(tokens, 2);
        return std::to_string(findAs<AVLTree>(storage, name, 'T', false)->height());
    }
    if (op == "LEN") {
        requireArgs(tokens, 2);
        return std::to_string(findAs<AVLTree>(storage, name, 'T', false)->size());
    }

    throw std::invalid_argument("unknown command: " + tokens.get(0));
}

static std::string executeSet(Storage& storage, const std::string& command, const Array& tokens, bool& changed) {
    requireArgs(tokens, 3);
    std::string name = tokens.get(1);
    std::string value = tokens.get(2);

    if (command == "SETADD") {
        changed = findAs<HashSet>(storage, name, 'H', true)->add(value);
        return value;
    }
    if (command == "SETDEL") {
        if (!findAs<HashSet>(storage, name, 'H', false)->remove(value)) {
            throw std::invalid_argument("value not found: " + value);
        }
        changed = true;
        return value;
    }
    return findAs<HashSet>(storage, name, 'H', false)->contains(value) ? "TRUE" : "FALSE";
}

std::string execute(Storage& storage, const Array& tokens, bool& changed) {
    if (tokens.size() < 2) {
        throw std::invalid_argument("expected: COMMAND name [args...]");
    }

    std::string command = toUpper(tokens.get(0));
    if (command.size() < 2) {
        throw std::invalid_argument("unknown command: " + tokens.get(0));
    }

    if (command == "PRINT") {
        requireArgs(tokens, 2);
        Container* container = storage.find(tokens.get(1));
        if (container == nullptr) {
            throw std::invalid_argument("structure not found: " + tokens.get(1));
        }
        return show(*container);
    }

    if (command == "ISMEMBER") {
        requireArgs(tokens, 3);
        return findAs<AVLTree>(storage, tokens.get(1), 'T', false)->find(tokens.get(2)) ? "TRUE" : "FALSE";
    }

    if (command == "SETADD" || command == "SETDEL" || command == "SET_AT") {
        return executeSet(storage, command, tokens, changed);
    }

    std::string op = command.substr(1);
    switch (command[0]) {
        case 'M': return executeArray(storage, op, tokens, changed);
        case 'F': return executeList<ForwardList>(storage, 'F', op, tokens, changed);
        case 'L': return executeList<DoublyList>(storage, 'L', op, tokens, changed);
        case 'S': return executeStack(storage, op, tokens, changed);
        case 'Q': return executeQueue(storage, op, tokens, changed);
        case 'T': return executeTree(storage, op, tokens, changed);
        default:  throw std::invalid_argument("unknown command: " + tokens.get(0));
    }
}
