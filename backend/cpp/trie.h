// Trie Implementation for Category/Payee Autocomplete
// Data Structures & Applications Lab Project
// Operations: insert, prefix search

#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    std::string word;  // Store complete word at end node
    
    TrieNode() : isEndOfWord(false) {}
    
    ~TrieNode() {
        for (auto& pair : children) {
            delete pair.second;
        }
    }
};

class Trie {
private:
    TrieNode* root;
    int wordCount;
    
    // Helper: Collect all words from a given node
    void collectWords(TrieNode* node, std::vector<std::string>& result) const {
        if (!node) return;
        
        if (node->isEndOfWord) {
            result.push_back(node->word);
        }
        
        for (const auto& pair : node->children) {
            collectWords(pair.second, result);
        }
    }
    
    // Convert to lowercase for case-insensitive search
    std::string toLower(const std::string& str) const {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
    
public:
    Trie() : wordCount(0) {
        root = new TrieNode();
    }
    
    ~Trie() {
        delete root;
    }
    
    // Insert a word into the trie
    // Time Complexity: O(m) where m is the length of the word
    void insert(const std::string& word) {
        if (word.empty()) return;
        
        std::string lowerWord = toLower(word);
        TrieNode* current = root;
        
        for (char c : lowerWord) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        
        if (!current->isEndOfWord) {
            current->isEndOfWord = true;
            current->word = word;  // Store original case
            wordCount++;
        }
    }
    
    // Search for exact word
    // Time Complexity: O(m)
    bool search(const std::string& word) const {
        std::string lowerWord = toLower(word);
        TrieNode* current = root;
        
        for (char c : lowerWord) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children[c];
        }
        
        return current->isEndOfWord;
    }
    
    // Check if any word starts with prefix
    // Time Complexity: O(m)
    bool startsWith(const std::string& prefix) const {
        std::string lowerPrefix = toLower(prefix);
        TrieNode* current = root;
        
        for (char c : lowerPrefix) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children[c];
        }
        
        return true;
    }
    
    // Get all words with given prefix (autocomplete)
    // Time Complexity: O(m + n) where m is prefix length and n is number of results
    std::vector<std::string> getWordsWithPrefix(const std::string& prefix, int maxResults = 10) const {
        std::vector<std::string> result;
        
        if (prefix.empty()) {
            collectWords(root, result);
            if ((int)result.size() > maxResults) {
                result.resize(maxResults);
            }
            return result;
        }
        
        std::string lowerPrefix = toLower(prefix);
        TrieNode* current = root;
        
        // Navigate to the end of prefix
        for (char c : lowerPrefix) {
            if (current->children.find(c) == current->children.end()) {
                return result;  // No words with this prefix
            }
            current = current->children[c];
        }
        
        // Collect all words from this point
        collectWords(current, result);
        
        // Limit results
        if ((int)result.size() > maxResults) {
            result.resize(maxResults);
        }
        
        return result;
    }
    
    // Remove a word from trie
    // Time Complexity: O(m)
    bool remove(const std::string& word) {
        std::string lowerWord = toLower(word);
        TrieNode* current = root;
        
        for (char c : lowerWord) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children[c];
        }
        
        if (current->isEndOfWord) {
            current->isEndOfWord = false;
            current->word = "";
            wordCount--;
            return true;
        }
        
        return false;
    }
    
    // Get all words in trie
    std::vector<std::string> getAllWords() const {
        std::vector<std::string> result;
        collectWords(root, result);
        return result;
    }
    
    int size() const { return wordCount; }
    bool isEmpty() const { return wordCount == 0; }
    
    void clear() {
        delete root;
        root = new TrieNode();
        wordCount = 0;
    }
};

#endif // TRIE_H
