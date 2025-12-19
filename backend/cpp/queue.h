// Queue Implementation for Upcoming Bill Payments (FIFO)
// Data Structures & Applications Lab Project
// Operations: enqueue, dequeue, peek

#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <vector>

struct Bill {
    std::string id;
    std::string name;
    double amount;
    std::string dueDate;
    std::string category;
    bool isPaid;
    
    Bill() : amount(0.0), isPaid(false) {}
    Bill(const std::string& i, const std::string& n, double a, 
         const std::string& d, const std::string& c)
        : id(i), name(n), amount(a), dueDate(d), category(c), isPaid(false) {}
};

struct QueueNode {
    Bill data;
    QueueNode* next;
    
    QueueNode(const Bill& b) : data(b), next(nullptr) {}
};

class BillQueue {
private:
    QueueNode* front;
    QueueNode* rear;
    int count;
    
public:
    BillQueue() : front(nullptr), rear(nullptr), count(0) {}
    
    ~BillQueue() {
        while (front) {
            QueueNode* temp = front;
            front = front->next;
            delete temp;
        }
    }
    
    // Add bill to the end of queue
    // Time Complexity: O(1)
    void enqueue(const Bill& b) {
        QueueNode* newNode = new QueueNode(b);
        
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }
    
    // Remove and return bill from front of queue
    // Time Complexity: O(1)
    bool dequeue(Bill& b) {
        if (!front) return false;
        
        b = front->data;
        QueueNode* temp = front;
        front = front->next;
        
        if (!front) {
            rear = nullptr;
        }
        
        delete temp;
        count--;
        return true;
    }
    
    // View front bill without removing
    // Time Complexity: O(1)
    bool peek(Bill& b) const {
        if (!front) return false;
        b = front->data;
        return true;
    }
    
    // Get all bills in queue (for display)
    // Time Complexity: O(n)
    std::vector<Bill> getAllBills() const {
        std::vector<Bill> result;
        QueueNode* current = front;
        
        while (current) {
            result.push_back(current->data);
            current = current->next;
        }
        return result;
    }
    
    // Find bill by ID
    // Time Complexity: O(n)
    bool findById(const std::string& id, Bill& result) const {
        QueueNode* current = front;
        
        while (current) {
            if (current->data.id == id) {
                result = current->data;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Remove bill by ID
    // Time Complexity: O(n)
    bool removeById(const std::string& id) {
        if (!front) return false;
        
        // Special case: front is the target
        if (front->data.id == id) {
            Bill temp;
            return dequeue(temp);
        }
        
        QueueNode* current = front;
        while (current->next) {
            if (current->next->data.id == id) {
                QueueNode* temp = current->next;
                current->next = temp->next;
                
                if (temp == rear) {
                    rear = current;
                }
                
                delete temp;
                count--;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Mark bill as paid by ID
    bool markAsPaid(const std::string& id) {
        QueueNode* current = front;
        
        while (current) {
            if (current->data.id == id) {
                current->data.isPaid = true;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    // Get unpaid bills
    std::vector<Bill> getUnpaidBills() const {
        std::vector<Bill> result;
        QueueNode* current = front;
        
        while (current) {
            if (!current->data.isPaid) {
                result.push_back(current->data);
            }
            current = current->next;
        }
        return result;
    }
    
    // Get overdue bills (due date < current date)
    std::vector<Bill> getOverdueBills(const std::string& currentDate) const {
        std::vector<Bill> result;
        QueueNode* current = front;
        
        while (current) {
            if (!current->data.isPaid && current->data.dueDate < currentDate) {
                result.push_back(current->data);
            }
            current = current->next;
        }
        return result;
    }
    
    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
    
    void clear() {
        while (front) {
            QueueNode* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
        count = 0;
    }
};

#endif // QUEUE_H
