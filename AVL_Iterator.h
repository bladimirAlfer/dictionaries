#include "AVL_Node.h"
#include <stack>
#include <queue>
using namespace std;

template <typename T>
class AVLIterator {
public:
    enum Type { PreOrder, InOrder, PostOrder, BFS };

private:
    NodeAVL<T>* current;
    Type type;
    stack<NodeAVL<T>*> st;
    stack<NodeAVL<T>*> post_helper;
    queue<NodeAVL<T>*> bfs_queue;

    void fillInOrder(NodeAVL<T>* node) {
        while (node) {
            st.push(node);
            node = node->left;
        }
    }

    void fillPostOrder(NodeAVL<T>* node) {
        if (!node) return;
        stack<NodeAVL<T>*> temp;
        temp.push(node);
        while (!temp.empty()) {
            NodeAVL<T>* curr = temp.top(); temp.pop();
            post_helper.push(curr);
            if (curr->left) temp.push(curr->left);
            if (curr->right) temp.push(curr->right);
        }
    }

    void fillPreOrder(NodeAVL<T>* node) {
        if (node) st.push(node);
    }

    void fillBFS(NodeAVL<T>* node) {
        if (node) bfs_queue.push(node);
    }

public:
    AVLIterator() : current(nullptr), type(InOrder) {}

    AVLIterator(std::nullptr_t) : current(nullptr), type(InOrder) {}

    AVLIterator(NodeAVL<T>* root, Type t) : current(nullptr), type(t) {
        switch (type) {
            case InOrder: fillInOrder(root); break;
            case PreOrder: fillPreOrder(root); break;
            case PostOrder: fillPostOrder(root); break;
            case BFS: fillBFS(root); break;
        }
        ++(*this); 
    }

    AVLIterator<T>& operator=(AVLIterator<T> other) {
        current = other.current;
        type = other.type;
        st = other.st;
        post_helper = other.post_helper;
        bfs_queue = other.bfs_queue;
        return *this;
    }

    bool operator!=(AVLIterator<T> other) {
        return current != other.current;
    }

    AVLIterator<T>& operator++() {
        current = nullptr;
        switch (type) {
            case InOrder:
                if (!st.empty()) {
                    current = st.top(); st.pop();
                    fillInOrder(current->right);
                }
                break;
            case PreOrder:
                if (!st.empty()) {
                    current = st.top(); st.pop();
                    if (current->right) st.push(current->right);
                    if (current->left) st.push(current->left);
                }
                break;
            case PostOrder:
                if (!post_helper.empty()) {
                    current = post_helper.top(); post_helper.pop();
                }
                break;
            case BFS:
                if (!bfs_queue.empty()) {
                    current = bfs_queue.front(); bfs_queue.pop();
                    if (current->left) bfs_queue.push(current->left);
                    if (current->right) bfs_queue.push(current->right);
                }
                break;
        }
        return *this;
    }

    T operator*() {
        return current->data;
    }
};
