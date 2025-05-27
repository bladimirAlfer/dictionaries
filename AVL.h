#include <iostream>
#include "AVL_Node.h"
#include "AVL_Iterator.h"
using namespace std;

template <typename T>
class AVLTree {
public:
    typedef AVLIterator<T> iterator;
    iterator begin(AVLIterator<T>::Type t) { return iterator(root, t); }
    iterator end() { return iterator(nullptr); }

private:
    NodeAVL<T>* root;
    int node_count = 0;

public:
    AVLTree() : root(nullptr) {}

    void insert(T value) { insert(root, value); }

    void insert(NodeAVL<T>*& node, T value) {
        if (!node) {
            node = new NodeAVL<T>(value);
            node_count++;
            return;
        }
        if (value < node->data)
            insert(node->left, value);
        else if (value > node->data)
            insert(node->right, value);
        else
            return;

        updateHeight(node);
        balance(node);
    }

    void remove(T value) { remove(root, value); }

    void remove(NodeAVL<T>*& node, T value) {
        if (!node) return;

        if (value < node->data)
            remove(node->left, value);
        else if (value > node->data)
            remove(node->right, value);
        else {
            if (!node->left || !node->right) {
                NodeAVL<T>* temp = node->left ? node->left : node->right;
                delete node;
                node = temp;
                node_count--;
            } else {
                NodeAVL<T>* pred = node->left;
                while (pred->right) pred = pred->right;
                node->data = pred->data;
                remove(node->left, pred->data);
            }
        }
        if (node) {
            updateHeight(node);
            balance(node);
        }
    }

    bool find(T value) {
        NodeAVL<T>* curr = root;
        while (curr) {
            if (value == curr->data) return true;
            curr = value < curr->data ? curr->left : curr->right;
        }
        return false;
    }

    T minValue() {
        NodeAVL<T>* curr = root;
        while (curr->left) curr = curr->left;
        return curr->data;
    }

    T maxValue() {
        NodeAVL<T>* curr = root;
        while (curr->right) curr = curr->right;
        return curr->data;
    }

    string getInOrder() {
        string res;
        inOrder(root, res);
        return res;
    }

    string getPreOrder() {
        string res;
        preOrder(root, res);
        return res;
    }

    string getPostOrder() {
        string res;
        postOrder(root, res);
        return res;
    }

    int height() { return root ? root->height : -1; }

    int size() { return node_count; }

    bool isBalanced() { return isBalanced(root); }

    bool isBalanced(NodeAVL<T>* node) {
        if (!node) return true;
        int bf = balancingFactor(node);
        return abs(bf) <= 1 && isBalanced(node->left) && isBalanced(node->right);
    }

    T successor(T value) {
        NodeAVL<T>* curr = root;
        NodeAVL<T>* succ = nullptr;
        while (curr) {
            if (value < curr->data) {
                succ = curr;
                curr = curr->left;
            } else
                curr = curr->right;
        }
        return succ ? succ->data : T();
    }

    T predecessor(T value) {
        NodeAVL<T>* curr = root;
        NodeAVL<T>* pred = nullptr;
        while (curr) {
            if (value > curr->data) {
                pred = curr;
                curr = curr->right;
            } else
                curr = curr->left;
        }
        return pred ? pred->data : T();
    }

    void clear() {
        if (root) {
            root->killSelf();
            root = nullptr;
            node_count = 0;
        }
    }

    void displayPretty() {
    }

private:
    void inOrder(NodeAVL<T>* node, string& res) {
        if (!node) return;
        inOrder(node->left, res);
        res += to_string(node->data) + " ";
        inOrder(node->right, res);
    }

    void preOrder(NodeAVL<T>* node, string& res) {
        if (!node) return;
        res += to_string(node->data) + " ";
        preOrder(node->left, res);
        preOrder(node->right, res);
    }

    void postOrder(NodeAVL<T>* node, string& res) {
        if (!node) return;
        postOrder(node->left, res);
        postOrder(node->right, res);
        res += to_string(node->data) + " ";
    }

    int balancingFactor(NodeAVL<T>* n) {
        return (n->left ? n->left->height + 1 : 0) - (n->right ? n->right->height + 1 : 0);
    }

    void updateHeight(NodeAVL<T>* n) {
        int lh = n->left ? n->left->height + 1 : 0;
        int rh = n->right ? n->right->height + 1 : 0;
        n->height = max(lh, rh);
    }

    void balance(NodeAVL<T>*& n) {
        int bf = balancingFactor(n);

        if (bf > 1) {
            if (balancingFactor(n->left) < 0)
                left_rota(n->left);
            right_rota(n);
        }
        else if (bf < -1) {
            if (balancingFactor(n->right) > 0)
                right_rota(n->right);
            left_rota(n);
        }
    }

    void left_rota(NodeAVL<T>*& n) {
        NodeAVL<T>* new_root = n->right;
        n->right = new_root->left;
        new_root->left = n;
        updateHeight(n);
        updateHeight(new_root);
        n = new_root;
    }

    void right_rota(NodeAVL<T>*& n) {
        NodeAVL<T>* new_root = n->left;
        n->left = new_root->right;
        new_root->right = n;
        updateHeight(n);
        updateHeight(new_root);
        n = new_root;
    }
};
