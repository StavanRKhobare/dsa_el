// Binary Search Tree Implementation for Date-wise Transaction Storage
// Data Structures & Applications Lab Project
// Operations: insert, in-order traversal, range query

#ifndef BST_H
#define BST_H

#include <string>
#include <vector>
#include "linkedlist.h"

struct BSTNode {
    std::string date;  // Format: YYYY-MM-DD for proper string comparison
    std::vector<Transaction> transactions;  // Multiple transactions per date
    BSTNode* left;
    BSTNode* right;
    
    BSTNode(const std::string& d) : date(d), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;
    int count;
    
    // Helper: Insert recursively
    BSTNode* insertHelper(BSTNode* node, const std::string& date, const Transaction& t) {
        if (!node) {
            BSTNode* newNode = new BSTNode(date);
            newNode->transactions.push_back(t);
            return newNode;
        }
        
        if (date < node->date) {
            node->left = insertHelper(node->left, date, t);
        } else if (date > node->date) {
            node->right = insertHelper(node->right, date, t);
        } else {
            // Same date, add transaction to existing node
            node->transactions.push_back(t);
        }
        
        return node;
    }
    
    // Helper: In-order traversal
    void inorderHelper(BSTNode* node, std::vector<Transaction>& result) const {
        if (!node) return;
        
        inorderHelper(node->left, result);
        for (const auto& t : node->transactions) {
            result.push_back(t);
        }
        inorderHelper(node->right, result);
    }
    
    // Helper: Reverse in-order (descending by date)
    void reverseInorderHelper(BSTNode* node, std::vector<Transaction>& result) const {
        if (!node) return;
        
        reverseInorderHelper(node->right, result);
        for (const auto& t : node->transactions) {
            result.push_back(t);
        }
        reverseInorderHelper(node->left, result);
    }
    
    // Helper: Range query
    void rangeQueryHelper(BSTNode* node, const std::string& startDate, 
                          const std::string& endDate, std::vector<Transaction>& result) const {
        if (!node) return;
        
        // If node's date is greater than start, search left subtree
        if (node->date > startDate) {
            rangeQueryHelper(node->left, startDate, endDate, result);
        }
        
        // If node's date is in range, add its transactions
        if (node->date >= startDate && node->date <= endDate) {
            for (const auto& t : node->transactions) {
                result.push_back(t);
            }
        }
        
        // If node's date is less than end, search right subtree
        if (node->date < endDate) {
            rangeQueryHelper(node->right, startDate, endDate, result);
        }
    }
    
    // Helper: Delete transaction by ID
    bool deleteTransactionHelper(BSTNode* node, const std::string& id) {
        if (!node) return false;
        
        // Search in current node
        for (auto it = node->transactions.begin(); it != node->transactions.end(); ++it) {
            if (it->id == id) {
                node->transactions.erase(it);
                count--;
                return true;
            }
        }
        
        // Search in subtrees
        if (deleteTransactionHelper(node->left, id)) return true;
        return deleteTransactionHelper(node->right, id);
    }
    
    // Helper: Clear tree
    void clearHelper(BSTNode* node) {
        if (!node) return;
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }
    
    // Helper: Find transaction by ID
    bool findHelper(BSTNode* node, const std::string& id, Transaction& result) const {
        if (!node) return false;
        
        for (const auto& t : node->transactions) {
            if (t.id == id) {
                result = t;
                return true;
            }
        }
        
        if (findHelper(node->left, id, result)) return true;
        return findHelper(node->right, id, result);
    }
    
public:
    BST() : root(nullptr), count(0) {}
    
    ~BST() {
        clearHelper(root);
    }
    
    // Insert transaction sorted by date
    // Time Complexity: O(log n) average, O(n) worst case
    void insert(const Transaction& t) {
        root = insertHelper(root, t.date, t);
        count++;
    }
    
    // In-order traversal (ascending by date)
    // Time Complexity: O(n)
    std::vector<Transaction> inorderTraversal() const {
        std::vector<Transaction> result;
        inorderHelper(root, result);
        return result;
    }
    
    // Reverse in-order traversal (descending by date)
    // Time Complexity: O(n)
    std::vector<Transaction> reverseInorderTraversal() const {
        std::vector<Transaction> result;
        reverseInorderHelper(root, result);
        return result;
    }
    
    // Range query: Get transactions between two dates
    // Time Complexity: O(log n + k) where k is number of results
    std::vector<Transaction> rangeQuery(const std::string& startDate, const std::string& endDate) const {
        std::vector<Transaction> result;
        rangeQueryHelper(root, startDate, endDate, result);
        return result;
    }
    
    // Delete transaction by ID
    // Time Complexity: O(n)
    bool deleteById(const std::string& id) {
        return deleteTransactionHelper(root, id);
    }
    
    // Find transaction by ID
    bool findById(const std::string& id, Transaction& result) const {
        return findHelper(root, id, result);
    }
    
    // Get transactions for a specific month (YYYY-MM)
    std::vector<Transaction> getByMonth(const std::string& yearMonth) const {
        std::string startDate = yearMonth + "-01";
        std::string endDate = yearMonth + "-31";  // Works due to string comparison
        return rangeQuery(startDate, endDate);
    }
    
    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
    
    void clear() {
        clearHelper(root);
        root = nullptr;
        count = 0;
    }
};

#endif // BST_H
