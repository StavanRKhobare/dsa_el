// Hash Map Implementation for Category → Budget Mapping
// Data Structures & Applications Lab Project
// Operations: insert, update, search, remove

#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>
#include <vector>
#include <iostream>

const int TABLE_SIZE = 100;

template<typename V>
struct HashNode {
    std::string key;
    V value;
    HashNode* next;
    
    HashNode(const std::string& k, const V& v) : key(k), value(v), next(nullptr) {}
};

template<typename V>
class HashMap {
private:
    std::vector<HashNode<V>*> table;
    int count;
    
    // Hash function using polynomial rolling hash
    int hash(const std::string& key) const {
        unsigned long hashVal = 0;
        int p = 31;
        int m = TABLE_SIZE;
        long p_pow = 1;
        
        for (char c : key) {
            hashVal = (hashVal + (c - 'a' + 1) * p_pow) % m;
            p_pow = (p_pow * p) % m;
        }
        return hashVal;
    }
    
public:
    HashMap() : count(0) {
        table.resize(TABLE_SIZE, nullptr);
    }
    
    ~HashMap() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode<V>* current = table[i];
            while (current) {
                HashNode<V>* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }
    
    // Insert or update key-value pair
    // Time Complexity: O(1) average, O(n) worst case
    void insert(const std::string& key, const V& value) {
        int index = hash(key);
        HashNode<V>* current = table[index];
        
        // Check if key exists, update if so
        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        // Key doesn't exist, insert new node
        HashNode<V>* newNode = new HashNode<V>(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        count++;
    }
    
    // Search for a key
    // Time Complexity: O(1) average, O(n) worst case
    bool search(const std::string& key, V& value) const {
        int index = hash(key);
        HashNode<V>* current = table[index];
        
        while (current) {
            if (current->key == key) {
                value = current->value;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Check if key exists
    bool contains(const std::string& key) const {
        int index = hash(key);
        HashNode<V>* current = table[index];
        
        while (current) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Remove a key
    // Time Complexity: O(1) average, O(n) worst case
    bool remove(const std::string& key) {
        int index = hash(key);
        HashNode<V>* current = table[index];
        HashNode<V>* prev = nullptr;
        
        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    table[index] = current->next;
                }
                delete current;
                count--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }
    
    // Update value for existing key
    bool update(const std::string& key, const V& value) {
        int index = hash(key);
        HashNode<V>* current = table[index];
        
        while (current) {
            if (current->key == key) {
                current->value = value;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Get all key-value pairs
    std::vector<std::pair<std::string, V>> getAllPairs() const {
        std::vector<std::pair<std::string, V>> pairs;
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode<V>* current = table[i];
            while (current) {
                pairs.push_back({current->key, current->value});
                current = current->next;
            }
        }
        return pairs;
    }
    
    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
};

#endif // HASHMAP_H
