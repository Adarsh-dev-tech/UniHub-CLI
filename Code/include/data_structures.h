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
// Trie Implementation
// ============================================================================
class Trie {
private:
    struct TrieNode {
        std::unordered_map<char, std::shared_ptr<TrieNode>> children;
        bool isEndOfWord;
        std::string fullWord;
        
        TrieNode() : isEndOfWord(false) {}
    };
    
    std::shared_ptr<TrieNode> root;
    
    void collectWords(std::shared_ptr<TrieNode> node, std::vector<std::string>& results) {
        if (!node) return;
        if (node->isEndOfWord) {
            results.push_back(node->fullWord);
        }
        for (auto& [ch, child] : node->children) {
            collectWords(child, results);
        }
    }

public:
    Trie() : root(std::make_shared<TrieNode>()) {}
    
    void insert(const std::string& word) {
        auto current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = std::make_shared<TrieNode>();
            }
            current = current->children[ch];
        }
        current->isEndOfWord = true;
        current->fullWord = word;
    }
    
    std::vector<std::string> getWordsWithPrefix(const std::string& prefix) {
        auto current = root;
        for (char ch : prefix) {
            if (current->children.find(ch) == current->children.end()) {
                return {}; // No words with this prefix
            }
            current = current->children[ch];
        }
        
        std::vector<std::string> results;
        collectWords(current, results);
        return results;
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