#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <queue>
#include <stack>
#include <list>
#include <functional>

namespace uni {

// Forward declarations
struct User;
struct Subject;
struct Resource;

// ============================================================================
// AVL Tree Implementation
// ============================================================================
template<typename T>
class AVLTree {
private:
    struct Node {
        T data;
        int height;
        std::shared_ptr<Node> left, right;
        
        Node(const T& val) : data(val), height(1), left(nullptr), right(nullptr) {}
    };
    
    using NodePtr = std::shared_ptr<Node>;
    NodePtr root;
    std::function<bool(const T&, const T&)> compare;
    
    int getHeight(NodePtr node) { return node ? node->height : 0; }
    int getBalance(NodePtr node) { return node ? getHeight(node->left) - getHeight(node->right) : 0; }
    
    void updateHeight(NodePtr node) {
        if (node) node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }
    
    NodePtr rotateRight(NodePtr y) {
        NodePtr x = y->left;
        NodePtr T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }
    
    NodePtr rotateLeft(NodePtr x) {
        NodePtr y = x->right;
        NodePtr T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }
    
    NodePtr insert(NodePtr node, const T& data) {
        if (!node) return std::make_shared<Node>(data);
        
        if (compare(data, node->data)) {
            node->left = insert(node->left, data);
        } else if (compare(node->data, data)) {
            node->right = insert(node->right, data);
        } else {
            return node; // Duplicate
        }
        
        updateHeight(node);
        int balance = getBalance(node);
        
        // Left Left Case
        if (balance > 1 && compare(data, node->left->data)) {
            return rotateRight(node);
        }
        // Right Right Case
        if (balance < -1 && compare(node->right->data, data)) {
            return rotateLeft(node);
        }
        // Left Right Case
        if (balance > 1 && compare(node->left->data, data)) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // Right Left Case
        if (balance < -1 && compare(data, node->right->data)) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }
    
    void inorder(NodePtr node, std::vector<T>& result) {
        if (node) {
            inorder(node->left, result);
            result.push_back(node->data);
            inorder(node->right, result);
        }
    }

public:
    AVLTree(std::function<bool(const T&, const T&)> comp = std::less<T>()) : root(nullptr), compare(comp) {}
    
    void insert(const T& data) { root = insert(root, data); }
    
    std::vector<T> getSorted() {
        std::vector<T> result;
        inorder(root, result);
        return result;
    }
};

// ============================================================================
// Binary Search Tree Implementation
// ============================================================================
template<typename T>
class BST {
private:
    struct Node {
        T data;
        std::shared_ptr<Node> left, right;
        
        Node(const T& val) : data(val), left(nullptr), right(nullptr) {}
    };
    
    using NodePtr = std::shared_ptr<Node>;
    NodePtr root;
    std::function<bool(const T&, const T&)> compare;
    
    NodePtr insert(NodePtr node, const T& data) {
        if (!node) return std::make_shared<Node>(data);
        
        if (compare(data, node->data)) {
            node->left = insert(node->left, data);
        } else if (compare(node->data, data)) {
            node->right = insert(node->right, data);
        }
        // Ignore duplicates
        
        return node;
    }
    
    void inorder(NodePtr node, std::vector<T>& result) {
        if (node) {
            inorder(node->left, result);
            result.push_back(node->data);
            inorder(node->right, result);
        }
    }
    
    void search(NodePtr node, const std::string& query, std::vector<T>& results) {
        if (!node) return;
        
        // For string-based search, we need to convert T to string for comparison
        // This is a simplified approach
        search(node->left, query, results);
        search(node->right, query, results);
    }

public:
    BST(std::function<bool(const T&, const T&)> comp = std::less<T>()) : root(nullptr), compare(comp) {}
    
    void insert(const T& data) { root = insert(root, data); }
    
    std::vector<T> getSorted() {
        std::vector<T> result;
        inorder(root, result);
        return result;
    }
    
    std::vector<T> searchAll() {
        return getSorted(); // Return all elements for simple search
    }
};

// ============================================================================
// Simple Array-based Autocomplete
// ============================================================================
class SimpleAutocomplete {
private:
    std::vector<std::string> words;

public:
    void insert(const std::string& word) {
        // Check if word already exists to avoid duplicates
        if (std::find(words.begin(), words.end(), word) == words.end()) {
            words.push_back(word);
            // Keep the array sorted for better search performance
            std::sort(words.begin(), words.end());
        }
    }
    
    std::vector<std::string> getWordsWithPrefix(const std::string& prefix) {
        std::vector<std::string> results;
        
        // Linear search through all words to find matches
        for (const auto& word : words) {
            if (word.length() >= prefix.length() && 
                word.substr(0, prefix.length()) == prefix) {
                results.push_back(word);
            }
        }
        
        return results;
    }
    
    std::vector<std::string> getAllWords() {
        return words;
    }
};

// ============================================================================
// DAG Implementation
// ============================================================================
template<typename T>
class DAG {
private:
    std::unordered_map<T, std::vector<T>> adjList;
    std::unordered_map<T, int> inDegree;

public:
    void addNode(const T& node) {
        if (adjList.find(node) == adjList.end()) {
            adjList[node] = {};
            inDegree[node] = 0;
        }
    }
    
    void addEdge(const T& from, const T& to) {
        addNode(from);
        addNode(to);
        adjList[from].push_back(to);
        inDegree[to]++;
    }
    
    std::vector<T> topologicalSort() {
        std::vector<T> result;
        std::queue<T> q;
        std::unordered_map<T, int> tempInDegree = inDegree;
        
        for (auto& [node, degree] : tempInDegree) {
            if (degree == 0) {
                q.push(node);
            }
        }
        
        while (!q.empty()) {
            T current = q.front();
            q.pop();
            result.push_back(current);
            
            for (const T& neighbor : adjList[current]) {
                tempInDegree[neighbor]--;
                if (tempInDegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }
        
        return result;
    }
    
    std::vector<T> getPrerequisites(const T& node) {
        std::vector<T> prereqs;
        for (auto& [from, toList] : adjList) {
            for (const T& to : toList) {
                if (to == node) {
                    prereqs.push_back(from);
                }
            }
        }
        return prereqs;
    }
};

// ============================================================================
// Graph Implementation
// ============================================================================
template<typename T>
class Graph {
private:
    std::unordered_map<T, std::vector<T>> adjList;

public:
    void addNode(const T& node) {
        if (adjList.find(node) == adjList.end()) {
            adjList[node] = {};
        }
    }
    
    void addEdge(const T& from, const T& to) {
        addNode(from);
        addNode(to);
        adjList[from].push_back(to);
        adjList[to].push_back(from); // Undirected
    }
    
    std::vector<T> getConnected(const T& node) {
        if (adjList.find(node) != adjList.end()) {
            return adjList[node];
        }
        return {};
    }
    
    std::vector<T> getAllNodes() {
        std::vector<T> nodes;
        for (auto& [node, _] : adjList) {
            nodes.push_back(node);
        }
        return nodes;
    }
};

}