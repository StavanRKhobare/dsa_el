// Stack Implementation for Undo Operations
// Data Structures & Applications Lab Project
// Operations: push, pop, peek

#ifndef STACK_H
#define STACK_H

#include <string>
#include <vector>
#include "linkedlist.h"

// Action types for undo
enum ActionType {
    ADD_TRANSACTION,
    DELETE_TRANSACTION,
    ADD_BUDGET,
    UPDATE_BUDGET,
    ADD_BILL,
    DELETE_BILL,
    PAY_BILL
};

struct Action {
    ActionType type;
    std::string data;  // JSON string of the affected data
    
    Action() : type(ADD_TRANSACTION) {}
    Action(ActionType t, const std::string& d) : type(t), data(d) {}
};

struct StackNode {
    Action data;
    StackNode* next;
    
    StackNode(const Action& a) : data(a), next(nullptr) {}
};

class UndoStack {
private:
    StackNode* top;
    int count;
    int maxSize;  // Limit stack size to prevent memory issues
    
public:
    UndoStack(int maxSz = 50) : top(nullptr), count(0), maxSize(maxSz) {}
    
    ~UndoStack() {
        while (top) {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
    }
    
    // Push action onto stack
    // Time Complexity: O(1)
    void push(const Action& a) {
        // If at max size, remove bottom element
        if (count >= maxSize) {
            // Find second to last
            StackNode* current = top;
            while (current->next && current->next->next) {
                current = current->next;
            }
            if (current->next) {
                delete current->next;
                current->next = nullptr;
                count--;
            }
        }
        
        StackNode* newNode = new StackNode(a);
        newNode->next = top;
        top = newNode;
        count++;
    }
    
    // Pop action from stack
    // Time Complexity: O(1)
    bool pop(Action& a) {
        if (!top) return false;
        
        a = top->data;
        StackNode* temp = top;
        top = top->next;
        delete temp;
        count--;
        return true;
    }
    
    // Peek at top action without removing
    // Time Complexity: O(1)
    bool peek(Action& a) const {
        if (!top) return false;
        a = top->data;
        return true;
    }
    
    // Get all actions (for visualization)
    // Time Complexity: O(n)
    std::vector<Action> getAllActions() const {
        std::vector<Action> result;
        StackNode* current = top;
        
        while (current) {
            result.push_back(current->data);
            current = current->next;
        }
        return result;
    }
    
    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
    
    void clear() {
        while (top) {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
        count = 0;
    }
};

// Simple transaction stack for recent transactions display
class TransactionStack {
private:
    StackNode* topNode;
    int count;
    int maxSize;
    
    struct TStackNode {
        Transaction data;
        TStackNode* next;
        
        TStackNode(const Transaction& t) : data(t), next(nullptr) {}
    };
    
    TStackNode* top;
    
public:
    TransactionStack(int maxSz = 100) : topNode(nullptr), count(0), maxSize(maxSz), top(nullptr) {}
    
    ~TransactionStack() {
        while (top) {
            TStackNode* temp = top;
            top = top->next;
            delete temp;
        }
    }
    
    void push(const Transaction& t) {
        TStackNode* newNode = new TStackNode(t);
        newNode->next = top;
        top = newNode;
        count++;
        
        // Trim if over max size
        if (count > maxSize) {
            TStackNode* current = top;
            for (int i = 0; i < maxSize - 1 && current; i++) {
                current = current->next;
            }
            if (current && current->next) {
                TStackNode* temp = current->next;
                current->next = nullptr;
                while (temp) {
                    TStackNode* toDelete = temp;
                    temp = temp->next;
                    delete toDelete;
                    count--;
                }
            }
        }
    }
    
    bool pop(Transaction& t) {
        if (!top) return false;
        
        t = top->data;
        TStackNode* temp = top;
        top = top->next;
        delete temp;
        count--;
        return true;
    }
    
    bool peek(Transaction& t) const {
        if (!top) return false;
        t = top->data;
        return true;
    }
    
    std::vector<Transaction> getAll() const {
        std::vector<Transaction> result;
        TStackNode* current = top;
        
        while (current) {
            result.push_back(current->data);
            current = current->next;
        }
        return result;
    }
    
    std::vector<Transaction> getTopN(int n) const {
        std::vector<Transaction> result;
        TStackNode* current = top;
        int i = 0;
        
        while (current && i < n) {
            result.push_back(current->data);
            current = current->next;
            i++;
        }
        return result;
    }
    
    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
    
    void clear() {
        while (top) {
            TStackNode* temp = top;
            top = top->next;
            delete temp;
        }
        count = 0;
    }
};

#endif // STACK_H
