#include <iostream>
using namespace std;

const int maxColision = 3;

template <typename TK, typename TV>
struct HashNode {
    TK key;
    TV value;
    HashNode* next;
    HashNode* insertion_next;

    HashNode(TK k, TV v) : key(k), value(v), next(nullptr), insertion_next(nullptr) {}
};

template <typename TK, typename TV>
class HashIterator {
private:
    HashNode<TK, TV>* current;
public:
    HashIterator(HashNode<TK, TV>* ptr = nullptr) : current(ptr) {}

    HashIterator& operator=(HashIterator other) {
        current = other.current;
        return *this;
    }

    bool operator!=(HashIterator other) {
        return current != other.current;
    }

    HashIterator& operator++() {
        if (current) current = current->insertion_next;
        return *this;
    }

    pair<TK, TV> operator*() {
        return { current->key, current->value };
    }

};

template <typename TK, typename TV>
class HashTable {
public:
    typedef HashIterator<TK, TV> iterator;
    iterator begin() { return iterator(insertion_head); }
    iterator end() { return iterator(nullptr); }

private:
    int capacity;
    int size;
    HashNode<TK, TV>** table;
    HashNode<TK, TV>* insertion_head;
    HashNode<TK, TV>* insertion_tail;

    int hash(TK key) {
        return std::hash<TK>{}(key) % capacity;
    }

    void rehashing() {
        int old_capacity = capacity;
        capacity *= 2;
        HashNode<TK, TV>** old_table = table;
        table = new HashNode<TK, TV>*[capacity]();

        HashNode<TK, TV>* temp = insertion_head;
        insertion_head = insertion_tail = nullptr;
        size = 0;
        while (temp) {
            insert(temp->key, temp->value);
            temp = temp->insertion_next;
        }

        for (int i = 0; i < old_capacity; ++i)
            delete old_table[i];
        delete[] old_table;
    }

public:
    HashTable(int _cap = 5) : capacity(_cap), size(0) {
        table = new HashNode<TK, TV>*[capacity]();
        insertion_head = insertion_tail = nullptr;
    }

    void insert(TK key, TV value) {
        int idx = hash(key);
        HashNode<TK, TV>* node = table[idx];
        int collisions = 0;

        while (node) {
            if (node->key == key) {
                node->value = value;
                return;
            }
            node = node->next;
            collisions++;
        }

        HashNode<TK, TV>* newNode = new HashNode<TK, TV>(key, value);
        newNode->next = table[idx];
        table[idx] = newNode;

        if (!insertion_head) insertion_head = insertion_tail = newNode;
        else {
            insertion_tail->insertion_next = newNode;
            insertion_tail = newNode;
        }

        size++;
        if (collisions >= maxColision)
            rehashing();
    }

    void insert(pair<TK, TV> item) {
        insert(item.first, item.second);
    }

    TV& at(TK key) {
        int idx = hash(key);
        HashNode<TK, TV>* node = table[idx];
        while (node) {
            if (node->key == key) return node->value;
            node = node->next;
        }
        throw out_of_range("Key not found");
    }

    TV& operator[](TK key) {
        int idx = hash(key);
        HashNode<TK, TV>* node = table[idx];
        while (node) {
            if (node->key == key) return node->value;
            node = node->next;
        }
        insert(key, TV());
        return at(key);
    }

    bool find(TK key) {
        int idx = hash(key);
        HashNode<TK, TV>* node = table[idx];
        while (node) {
            if (node->key == key) return true;
            node = node->next;
        }
        return false;
    }

    bool remove(TK key) {
        int idx = hash(key);
        HashNode<TK, TV>*& head = table[idx];
        HashNode<TK, TV>* prev = nullptr;
        HashNode<TK, TV>* node = head;
        while (node) {
            if (node->key == key) {
                if (prev) prev->next = node->next;
                else head = node->next;

                if (node == insertion_head) insertion_head = node->insertion_next;
                else {
                    HashNode<TK, TV>* it = insertion_head;
                    while (it && it->insertion_next != node)
                        it = it->insertion_next;
                    if (it) it->insertion_next = node->insertion_next;
                    if (node == insertion_tail) insertion_tail = it;
                }

                delete node;
                size--;
                return true;
            }
            prev = node;
            node = node->next;
        }
        return false;
    }

    int getSize() { return size; }
};

