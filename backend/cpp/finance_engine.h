// Finance Engine - Main Integration Layer
// Combines all data structures for the Finance Tracker
// Data Structures & Applications Lab Project

#ifndef FINANCE_ENGINE_H
#define FINANCE_ENGINE_H

#include "hashmap.h"
#include "linkedlist.h"
#include "bst.h"
#include "heap.h"
#include "queue.h"
#include "stack.h"
#include "trie.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>

// Budget structure
struct Budget {
    std::string category;
    double limit;
    double spent;
    
    Budget() : limit(0.0), spent(0.0) {}
    Budget(const std::string& c, double l, double s = 0.0) 
        : category(c), limit(l), spent(s) {}
    
    double getPercentUsed() const {
        if (limit == 0) return 0;
        return (spent / limit) * 100;
    }
    
    std::string getAlertLevel() const {
        double percent = getPercentUsed();
        if (percent >= 100) return "exceeded";
        if (percent >= 80) return "warning";
        if (percent >= 50) return "caution";
        return "normal";
    }
};

// Monthly summary structure
struct MonthlySummary {
    std::string month;  // YYYY-MM format
    double totalIncome;
    double totalExpenses;
    double netSavings;
    int transactionCount;
    std::vector<std::pair<std::string, double>> categoryBreakdown;
    
    MonthlySummary() : totalIncome(0), totalExpenses(0), netSavings(0), transactionCount(0) {}
};

// Budget alert structure
struct BudgetAlert {
    std::string category;
    std::string level;      // "caution", "warning", "exceeded"
    double percentUsed;
    double spent;
    double limit;
    std::string message;
};

class FinanceEngine {
private:
    // Data Structures
    HashMap<Budget> budgetMap;           // Category → Budget mapping
    HashMap<double> expenseMap;          // Category → Total expenses
    DoublyLinkedList transactionList;    // Transaction history
    BST transactionBST;                  // Date-sorted transactions
    MaxHeap expenseHeap;                 // For top expenses
    CategoryMaxHeap categoryHeap;        // For top spending categories
    BillQueue billQueue;                 // Upcoming bills
    UndoStack undoStack;                 // Undo operations
    TransactionStack recentStack;        // Recent transactions
    Trie categoryTrie;                   // Category autocomplete
    Trie payeeTrie;                      // Payee/description autocomplete
    
    // Generate unique ID
    std::string generateId() {
        static int counter = 0;
        std::stringstream ss;
        ss << "txn_" << std::time(nullptr) << "_" << (++counter);
        return ss.str();
    }
    
    std::string generateBillId() {
        static int counter = 0;
        std::stringstream ss;
        ss << "bill_" << std::time(nullptr) << "_" << (++counter);
        return ss.str();
    }
    
    // Update expense tracking after transaction
    void updateExpenseTracking(const Transaction& t, bool isAdd) {
        if (t.type != "expense") return;
        
        double current = 0;
        expenseMap.search(t.category, current);
        
        if (isAdd) {
            expenseMap.insert(t.category, current + t.amount);
            
            // Update budget spent
            Budget b;
            if (budgetMap.search(t.category, b)) {
                b.spent = current + t.amount;
                budgetMap.update(t.category, b);
            }
        } else {
            expenseMap.insert(t.category, std::max(0.0, current - t.amount));
            
            // Update budget spent
            Budget b;
            if (budgetMap.search(t.category, b)) {
                b.spent = std::max(0.0, current - t.amount);
                budgetMap.update(t.category, b);
            }
        }
    }
    
public:
    FinanceEngine() {
        // Initialize with default categories
        std::vector<std::string> defaultCategories = {
            "Food", "Transport", "Shopping", "Entertainment", "Bills",
            "Healthcare", "Education", "Salary", "Freelance", "Investment",
            "Rent", "Utilities", "Groceries", "Dining", "Travel"
        };
        
        for (const auto& cat : defaultCategories) {
            categoryTrie.insert(cat);
        }
    }
    
    // ===== TRANSACTION OPERATIONS =====
    
    // Add a new transaction
    Transaction addTransaction(const std::string& type, double amount, 
                               const std::string& category, const std::string& description,
                               const std::string& date) {
        Transaction t(generateId(), type, amount, category, description, date);
        
        // Add to data structures
        transactionList.addFront(t);
        transactionBST.insert(t);
        recentStack.push(t);
        
        // Update expense tracking
        updateExpenseTracking(t, true);
        
        // Add to heap if expense
        if (type == "expense") {
            expenseHeap.insert(t);
        }
        
        // Add to tries
        categoryTrie.insert(category);
        if (!description.empty()) {
            payeeTrie.insert(description);
        }
        
        // Record for undo
        std::stringstream ss;
        ss << t.id << "|" << t.type << "|" << t.amount << "|" 
           << t.category << "|" << t.description << "|" << t.date;
        undoStack.push(Action(ADD_TRANSACTION, ss.str()));
        
        return t;
    }
    
    // Delete transaction by ID
    bool deleteTransaction(const std::string& id) {
        Transaction t;
        if (!transactionBST.findById(id, t)) {
            return false;
        }
        
        // Record for undo before deleting
        std::stringstream ss;
        ss << t.id << "|" << t.type << "|" << t.amount << "|" 
           << t.category << "|" << t.description << "|" << t.date;
        undoStack.push(Action(DELETE_TRANSACTION, ss.str()));
        
        // Remove from data structures
        transactionList.deleteById(id);
        transactionBST.deleteById(id);
        
        // Update expense tracking
        updateExpenseTracking(t, false);
        
        return true;
    }
    
    // Get all transactions
    std::vector<Transaction> getAllTransactions() const {
        return transactionList.traverseForward();
    }
    
    // Get transactions sorted by date (ascending)
    std::vector<Transaction> getTransactionsByDateAsc() const {
        return transactionBST.inorderTraversal();
    }
    
    // Get transactions sorted by date (descending)
    std::vector<Transaction> getTransactionsByDateDesc() const {
        return transactionBST.reverseInorderTraversal();
    }
    
    // Get transactions in date range
    std::vector<Transaction> getTransactionsInRange(const std::string& startDate, 
                                                     const std::string& endDate) const {
        return transactionBST.rangeQuery(startDate, endDate);
    }
    
    // Get recent transactions (from stack)
    std::vector<Transaction> getRecentTransactions(int count = 10) const {
        return recentStack.getTopN(count);
    }
    
    // Get transactions by category
    std::vector<Transaction> getTransactionsByCategory(const std::string& category) const {
        return transactionList.filterByCategory(category);
    }
    
    // ===== BUDGET OPERATIONS =====
    
    // Set budget for category
    void setBudget(const std::string& category, double limit) {
        Budget existing;
        double spent = 0;
        expenseMap.search(category, spent);
        
        if (budgetMap.search(category, existing)) {
            // Update existing
            std::stringstream ss;
            ss << category << "|" << existing.limit;
            undoStack.push(Action(UPDATE_BUDGET, ss.str()));
            
            existing.limit = limit;
            budgetMap.update(category, existing);
        } else {
            // Add new
            std::stringstream ss;
            ss << category << "|" << limit;
            undoStack.push(Action(ADD_BUDGET, ss.str()));
            
            Budget b(category, limit, spent);
            budgetMap.insert(category, b);
        }
        
        categoryTrie.insert(category);
    }
    
    // Get budget for category
    bool getBudget(const std::string& category, Budget& budget) const {
        return budgetMap.search(category, budget);
    }
    
    // Get all budgets
    std::vector<Budget> getAllBudgets() const {
        std::vector<Budget> result;
        auto pairs = budgetMap.getAllPairs();
        for (const auto& pair : pairs) {
            result.push_back(pair.second);
        }
        return result;
    }
    
    // Get budget alerts
    std::vector<BudgetAlert> getBudgetAlerts() const {
        std::vector<BudgetAlert> alerts;
        auto budgets = getAllBudgets();
        
        for (const auto& b : budgets) {
            if (b.getAlertLevel() != "normal") {
                BudgetAlert alert;
                alert.category = b.category;
                alert.level = b.getAlertLevel();
                alert.percentUsed = b.getPercentUsed();
                alert.spent = b.spent;
                alert.limit = b.limit;
                
                if (alert.level == "exceeded") {
                    alert.message = "Budget exceeded! You've spent $" + 
                                   std::to_string((int)b.spent) + " of $" + 
                                   std::to_string((int)b.limit);
                } else if (alert.level == "warning") {
                    alert.message = "Warning: 80%+ of budget used";
                } else {
                    alert.message = "Caution: 50%+ of budget used";
                }
                
                alerts.push_back(alert);
            }
        }
        
        return alerts;
    }
    
    // ===== BILL OPERATIONS =====
    
    // Add a bill
    Bill addBill(const std::string& name, double amount, 
                 const std::string& dueDate, const std::string& category) {
        Bill b(generateBillId(), name, amount, dueDate, category);
        billQueue.enqueue(b);
        
        std::stringstream ss;
        ss << b.id << "|" << b.name << "|" << b.amount << "|" << b.dueDate << "|" << b.category;
        undoStack.push(Action(ADD_BILL, ss.str()));
        
        return b;
    }
    
    // Get all bills
    std::vector<Bill> getAllBills() const {
        return billQueue.getAllBills();
    }
    
    // Get unpaid bills
    std::vector<Bill> getUnpaidBills() const {
        return billQueue.getUnpaidBills();
    }
    
    // Get overdue bills
    std::vector<Bill> getOverdueBills(const std::string& currentDate) const {
        return billQueue.getOverdueBills(currentDate);
    }
    
    // Mark bill as paid
    bool payBill(const std::string& id) {
        Bill b;
        if (billQueue.findById(id, b)) {
            undoStack.push(Action(PAY_BILL, id));
            return billQueue.markAsPaid(id);
        }
        return false;
    }
    
    // Remove bill
    bool removeBill(const std::string& id) {
        Bill b;
        if (billQueue.findById(id, b)) {
            std::stringstream ss;
            ss << b.id << "|" << b.name << "|" << b.amount << "|" << b.dueDate << "|" << b.category;
            undoStack.push(Action(DELETE_BILL, ss.str()));
            return billQueue.removeById(id);
        }
        return false;
    }
    
    // Get next bill (peek)
    bool getNextBill(Bill& bill) const {
        return billQueue.peek(bill);
    }
    
    // ===== ANALYTICS =====
    
    // Get top expenses (using heap)
    std::vector<Transaction> getTopExpenses(int k = 5) {
        // Rebuild heap with current expenses
        auto expenses = transactionList.filterByType("expense");
        expenseHeap.clear();
        expenseHeap.buildHeap(expenses);
        return expenseHeap.getTopK(k);
    }
    
    // Get top spending categories
    std::vector<CategoryAmount> getTopCategories(int k = 5) {
        auto pairs = expenseMap.getAllPairs();
        std::vector<CategoryAmount> categories;
        
        for (const auto& pair : pairs) {
            if (pair.second > 0) {
                categories.push_back(CategoryAmount(pair.first, pair.second));
            }
        }
        
        categoryHeap.clear();
        categoryHeap.buildHeap(categories);
        return categoryHeap.getTopK(k);
    }
    
    // Get monthly summary
    MonthlySummary getMonthlySummary(const std::string& yearMonth) const {
        MonthlySummary summary;
        summary.month = yearMonth;
        
        auto transactions = transactionBST.getByMonth(yearMonth);
        std::unordered_map<std::string, double> categoryTotals;
        
        for (const auto& t : transactions) {
            summary.transactionCount++;
            
            if (t.type == "income") {
                summary.totalIncome += t.amount;
            } else {
                summary.totalExpenses += t.amount;
                categoryTotals[t.category] += t.amount;
            }
        }
        
        summary.netSavings = summary.totalIncome - summary.totalExpenses;
        
        for (const auto& pair : categoryTotals) {
            summary.categoryBreakdown.push_back(pair);
        }
        
        return summary;
    }
    
    // ===== AUTOCOMPLETE =====
    
    // Get category suggestions
    std::vector<std::string> getCategorySuggestions(const std::string& prefix) const {
        return categoryTrie.getWordsWithPrefix(prefix);
    }
    
    // Get all categories
    std::vector<std::string> getAllCategories() const {
        return categoryTrie.getAllWords();
    }
    
    // Get payee/description suggestions
    std::vector<std::string> getPayeeSuggestions(const std::string& prefix) const {
        return payeeTrie.getWordsWithPrefix(prefix);
    }
    
    // ===== UNDO OPERATIONS =====
    
    // Perform undo
    bool undo() {
        Action action;
        if (!undoStack.pop(action)) {
            return false;
        }
        
        std::stringstream ss(action.data);
        std::string token;
        
        switch (action.type) {
            case ADD_TRANSACTION: {
                // Undo add = delete
                std::getline(ss, token, '|');  // id
                deleteTransaction(token);
                // Pop the delete action that was just added
                Action temp;
                undoStack.pop(temp);
                break;
            }
            case DELETE_TRANSACTION: {
                // Undo delete = add back
                std::string id, type, category, description, date;
                double amount;
                std::getline(ss, id, '|');
                std::getline(ss, type, '|');
                ss >> amount;
                ss.ignore();
                std::getline(ss, category, '|');
                std::getline(ss, description, '|');
                std::getline(ss, date);
                
                Transaction t(id, type, amount, category, description, date);
                transactionList.addFront(t);
                transactionBST.insert(t);
                updateExpenseTracking(t, true);
                break;
            }
            case ADD_BUDGET: {
                // Undo add budget = remove
                std::getline(ss, token, '|');  // category
                budgetMap.remove(token);
                break;
            }
            case UPDATE_BUDGET: {
                // Undo update = restore old value
                std::string category;
                double oldLimit;
                std::getline(ss, category, '|');
                ss >> oldLimit;
                
                Budget b;
                if (budgetMap.search(category, b)) {
                    b.limit = oldLimit;
                    budgetMap.update(category, b);
                }
                break;
            }
            default:
                break;
        }
        
        return true;
    }
    
    // Check if undo is available
    bool canUndo() const {
        Action temp;
        return undoStack.isEmpty() == false;
    }
    
    // ===== DATA LOADING =====
    
    // Load transaction from parsed data
    void loadTransaction(const std::string& id, const std::string& type, double amount,
                         const std::string& category, const std::string& description,
                         const std::string& date) {
        Transaction t(id, type, amount, category, description, date);
        transactionList.addBack(t);
        transactionBST.insert(t);
        recentStack.push(t);
        updateExpenseTracking(t, true);
        
        if (type == "expense") {
            expenseHeap.insert(t);
        }
        
        categoryTrie.insert(category);
        if (!description.empty()) {
            payeeTrie.insert(description);
        }
    }
    
    // Load budget from parsed data
    void loadBudget(const std::string& category, double limit) {
        double spent = 0;
        expenseMap.search(category, spent);
        Budget b(category, limit, spent);
        budgetMap.insert(category, b);
        categoryTrie.insert(category);
    }
    
    // Load bill from parsed data
    void loadBill(const std::string& id, const std::string& name, double amount,
                  const std::string& dueDate, const std::string& category, bool isPaid) {
        Bill b(id, name, amount, dueDate, category);
        b.isPaid = isPaid;
        billQueue.enqueue(b);
    }
    
    // Clear all data
    void clearAll() {
        transactionList.clear();
        transactionBST.clear();
        expenseHeap.clear();
        recentStack.clear();
        undoStack.clear();
        billQueue.clear();
    }
    
    // ===== STATISTICS =====
    
    int getTransactionCount() const { return transactionList.size(); }
    int getBudgetCount() const { return budgetMap.size(); }
    int getBillCount() const { return billQueue.size(); }
    
    // Get total balance
    double getTotalBalance() const {
        auto transactions = transactionList.traverseForward();
        double balance = 0;
        
        for (const auto& t : transactions) {
            if (t.type == "income") {
                balance += t.amount;
            } else {
                balance -= t.amount;
            }
        }
        
        return balance;
    }
    
    // Get total income
    double getTotalIncome() const {
        auto transactions = transactionList.filterByType("income");
        double total = 0;
        for (const auto& t : transactions) {
            total += t.amount;
        }
        return total;
    }
    
    // Get total expenses
    double getTotalExpenses() const {
        auto transactions = transactionList.filterByType("expense");
        double total = 0;
        for (const auto& t : transactions) {
            total += t.amount;
        }
        return total;
    }
};

#endif // FINANCE_ENGINE_H
