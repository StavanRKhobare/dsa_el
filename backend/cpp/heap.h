// Max Heap Implementation for Top Spending Transactions
// Data Structures & Applications Lab Project
// Operations: insert, extract max, heapify, peek

#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <string>
#include "linkedlist.h"

class MaxHeap {
private:
    std::vector<Transaction> heap;
    
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }
    
    // Heapify up (for insertion)
    // Time Complexity: O(log n)
    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)].amount < heap[i].amount) {
            std::swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }
    
    // Heapify down (for extraction)
    // Time Complexity: O(log n)
    void heapifyDown(int i) {
        int largest = i;
        int left = leftChild(i);
        int right = rightChild(i);
        
        if (left < (int)heap.size() && heap[left].amount > heap[largest].amount) {
            largest = left;
        }
        
        if (right < (int)heap.size() && heap[right].amount > heap[largest].amount) {
            largest = right;
        }
        
        if (largest != i) {
            std::swap(heap[i], heap[largest]);
            heapifyDown(largest);
        }
    }
    
public:
    MaxHeap() {}
    
    // Build heap from array of transactions
    // Time Complexity: O(n)
    void buildHeap(const std::vector<Transaction>& transactions) {
        heap = transactions;
        // Start from last non-leaf node and heapify down
        for (int i = (int)heap.size() / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }
    
    // Insert transaction into heap
    // Time Complexity: O(log n)
    void insert(const Transaction& t) {
        heap.push_back(t);
        heapifyUp(heap.size() - 1);
    }
    
    // Extract max (highest amount transaction)
    // Time Complexity: O(log n)
    bool extractMax(Transaction& result) {
        if (heap.empty()) return false;
        
        result = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        
        if (!heap.empty()) {
            heapifyDown(0);
        }
        
        return true;
    }
    
    // Peek at max without removing
    // Time Complexity: O(1)
    bool peek(Transaction& result) const {
        if (heap.empty()) return false;
        result = heap[0];
        return true;
    }
    
    // Get top K highest transactions
    // Time Complexity: O(k log n)
    std::vector<Transaction> getTopK(int k) {
        std::vector<Transaction> result;
        std::vector<Transaction> backup = heap;
        
        for (int i = 0; i < k && !heap.empty(); i++) {
            Transaction t;
            if (extractMax(t)) {
                result.push_back(t);
            }
        }
        
        // Restore heap
        heap = backup;
        
        return result;
    }
    
    // Get all transactions in heap order (for visualization)
    std::vector<Transaction> getHeapArray() const {
        return heap;
    }
    
    int size() const { return heap.size(); }
    bool isEmpty() const { return heap.empty(); }
    
    void clear() {
        heap.clear();
    }
};

// Category-based heap for top spending categories
struct CategoryAmount {
    std::string category;
    double totalAmount;
    
    CategoryAmount() : totalAmount(0.0) {}
    CategoryAmount(const std::string& c, double a) : category(c), totalAmount(a) {}
};

class CategoryMaxHeap {
private:
    std::vector<CategoryAmount> heap;
    
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }
    
    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)].totalAmount < heap[i].totalAmount) {
            std::swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }
    
    void heapifyDown(int i) {
        int largest = i;
        int left = leftChild(i);
        int right = rightChild(i);
        
        if (left < (int)heap.size() && heap[left].totalAmount > heap[largest].totalAmount) {
            largest = left;
        }
        
        if (right < (int)heap.size() && heap[right].totalAmount > heap[largest].totalAmount) {
            largest = right;
        }
        
        if (largest != i) {
            std::swap(heap[i], heap[largest]);
            heapifyDown(largest);
        }
    }
    
public:
    void buildHeap(const std::vector<CategoryAmount>& categories) {
        heap = categories;
        for (int i = (int)heap.size() / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }
    
    void insert(const CategoryAmount& ca) {
        heap.push_back(ca);
        heapifyUp(heap.size() - 1);
    }
    
    bool extractMax(CategoryAmount& result) {
        if (heap.empty()) return false;
        
        result = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        
        if (!heap.empty()) {
            heapifyDown(0);
        }
        
        return true;
    }
    
    std::vector<CategoryAmount> getTopK(int k) {
        std::vector<CategoryAmount> result;
        std::vector<CategoryAmount> backup = heap;
        
        for (int i = 0; i < k && !heap.empty(); i++) {
            CategoryAmount ca;
            if (extractMax(ca)) {
                result.push_back(ca);
            }
        }
        
        heap = backup;
        return result;
    }
    
    int size() const { return heap.size(); }
    bool isEmpty() const { return heap.empty(); }
    void clear() { heap.clear(); }
};

#endif // HEAP_H
