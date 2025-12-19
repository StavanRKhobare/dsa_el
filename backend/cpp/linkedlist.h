// Doubly Linked List Implementation for Transaction History
// Data Structures & Applications Lab Project
// Operations: add, delete, traverse forward/backward

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>
#include <vector>
#include <functional>

struct Transaction {
    std::string id;
    std::string type;      // "income" or "expense"
    double amount;
    std::string category;
    std::string description;
    std::string date;
    
    Transaction() : amount(0.0) {}
    Transaction(const std::string& i, const std::string& t, double a, 
                const std::string& c, const std::string& d, const std::string& dt)
        : id(i), type(t), amount(a), category(c), description(d), date(dt) {}
};

struct DLLNode {
    Transaction data;
    DLLNode* prev;
    DLLNode* next;
    
    DLLNode(const Transaction& t) : data(t), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    DLLNode* head;
    DLLNode* tail;
    int count;
    
public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}
    
    ~DoublyLinkedList() {
        DLLNode* current = head;
        while (current) {
            DLLNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    
    // Add transaction to the front (most recent)
    // Time Complexity: O(1)
    void addFront(const Transaction& t) {
        DLLNode* newNode = new DLLNode(t);
        
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }
    
    // Add transaction to the back
    // Time Complexity: O(1)
    void addBack(const Transaction& t) {
        DLLNode* newNode = new DLLNode(t);
        
        if (!tail) {
            head = tail = newNode;
        } else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }
    
    // Delete transaction by ID
    // Time Complexity: O(n)
    bool deleteById(const std::string& id) {
        DLLNode* current = head;
        
        while (current) {
            if (current->data.id == id) {
                if (current->prev) {
                    current->prev->next = current->next;
                } else {
                    head = current->next;
                }
                
                if (current->next) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;
                }
                
                delete current;
                count--;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Delete and return the most recent transaction (for undo)
    // Time Complexity: O(1)
    bool deleteFront(Transaction& t) {
        if (!head) return false;
        
        t = head->data;
        DLLNode* temp = head;
        
        head = head->next;
        if (head) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        
        delete temp;
        count--;
        return true;
    }
    
    // Traverse forward and collect all transactions
    // Time Complexity: O(n)
    std::vector<Transaction> traverseForward() const {
        std::vector<Transaction> result;
        DLLNode* current = head;
        
        while (current) {
            result.push_back(current->data);
            current = current->next;
        }
        return result;
    }
    
    // Traverse backward
    // Time Complexity: O(n)
    std::vector<Transaction> traverseBackward() const {
        std::vector<Transaction> result;
        DLLNode* current = tail;
        
        while (current) {
            result.push_back(current->data);
            current = current->prev;
        }
        return result;
    }
    
    // Find transaction by ID
    // Time Complexity: O(n)
    bool findById(const std::string& id, Transaction& t) const {
        DLLNode* current = head;
        
        while (current) {
            if (current->data.id == id) {
                t = current->data;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Get most recent transaction
    bool getFront(Transaction& t) const {
        if (!head) return false;
        t = head->data;
        return true;
    }
    
    // Filter transactions by category
    std::vector<Transaction> filterByCategory(const std::string& category) const {
        std::vector<Transaction> result;
        DLLNode* current = head;
        
        while (current) {
            if (current->data.category == category) {
                result.push_back(current->data);
            }
            current = current->next;
        }
        return result;
    }
    
    // Filter transactions by type
    std::vector<Transaction> filterByType(const std::string& type) const {
        std::vector<Transaction> result;
        DLLNode* current = head;
        
        while (current) {
            if (current->data.type == type) {
                result.push_back(current->data);
            }
            current = current->next;
        }
        return result;
    }
    
    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
    
    // Clear all transactions
    void clear() {
        DLLNode* current = head;
        while (current) {
            DLLNode* temp = current;
            current = current->next;
            delete temp;
        }
        head = tail = nullptr;
        count = 0;
    }
};

#endif // LINKEDLIST_H
