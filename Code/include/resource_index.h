#pragma once
#include "resources.h"
#include "data_structures.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <memory>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <optional>

namespace uni {

// ============================================================================
// Enhanced Resource Management with Simple Data Structures
// ============================================================================

struct ResourceMetadata {
    std::string filename;
    std::string displayName;
    std::string filePath;
    std::string resourceType;
    std::string subject;
    std::string uploader;
    std::size_t sizeBytes;
    std::chrono::system_clock::time_point uploadTime;
    int downloadCount;
    double rating;
    std::vector<std::string> tags;
    
    ResourceMetadata() : sizeBytes(0), uploadTime(std::chrono::system_clock::now()),
                        downloadCount(0), rating(0.0) {}
    
    bool operator<(const ResourceMetadata& other) const {
        // For priority queue (higher priority = more popular)
        return downloadCount < other.downloadCount;
    }
    
    // For BST comparison (sort by filename)
    bool operator<(const std::string& filename) const {
        return this->filename < filename;
    }
};

class ResourceIndex {
private:
    // BST: Efficient file metadata storage and search (instead of B-Tree)
    BST<ResourceMetadata> resourceBST;
    
    // Simple Array: Autocomplete for resource names (instead of Trie)
    SimpleAutocomplete resourceNameAutocomplete;
    
    // Priority Queue: Most popular resources
    std::priority_queue<ResourceMetadata> popularResources;
    
    // Graph: Resource relationships (similar content, references)
    Graph<std::string> resourceGraph;
    
    // Hash Maps: Various indexes
    std::unordered_map<std::string, ResourceMetadata> filenameIndex;
    std::unordered_map<std::string, std::vector<std::string>> tagIndex;
    std::unordered_map<std::string, std::vector<std::string>> uploaderIndex;
    
    // Inverted Index: Full-text search capability
    std::unordered_map<std::string, std::vector<std::string>> invertedIndex;
    
    void updateInvertedIndex(const ResourceMetadata& resource) {
        // Simple tokenization and indexing
        std::string text = resource.displayName + " " + resource.subject + " " + resource.resourceType;
        std::istringstream iss(text);
        std::string word;
        
        while (iss >> word) {
            // Convert to lowercase and remove punctuation
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
            
            if (!word.empty()) {
                invertedIndex[word].push_back(resource.filename);
            }
        }
        
        // Index tags
        for (const auto& tag : resource.tags) {
            std::string lowerTag = tag;
            std::transform(lowerTag.begin(), lowerTag.end(), lowerTag.begin(), ::tolower);
            invertedIndex[lowerTag].push_back(resource.filename);
        }
    }

public:
    ResourceIndex() : resourceBST([](const ResourceMetadata& a, const ResourceMetadata& b) { 
        return a.filename < b.filename; 
    }) {}
    
    void addResource(const ResourceMetadata& resource) {
        // Add to all data structures
        filenameIndex[resource.filename] = resource;
        
        // Add to BST (instead of B-Tree)
        resourceBST.insert(resource);
        
        // Add to Simple Autocomplete (instead of Trie)
        resourceNameAutocomplete.insert(resource.displayName);
        
        // Add to popularity queue
        popularResources.push(resource);
        
        // Add to graph
        resourceGraph.addNode(resource.filename);
        
        // Index by tags
        for (const auto& tag : resource.tags) {
            tagIndex[tag].push_back(resource.filename);
        }
        
        // Index by uploader
        uploaderIndex[resource.uploader].push_back(resource.filename);
        
        // Update inverted index
        updateInvertedIndex(resource);
    }
    
    std::vector<std::string> autocompleteResourceName(const std::string& prefix) {
        return resourceNameAutocomplete.getWordsWithPrefix(prefix);
    }
    
    std::vector<ResourceMetadata> getPopularResources(int count = 10) {
        std::vector<ResourceMetadata> result;
        auto tempQueue = popularResources;
        
        for (int i = 0; i < count && !tempQueue.empty(); ++i) {
            result.push_back(tempQueue.top());
            tempQueue.pop();
        }
        
        return result;
    }
    
    std::vector<ResourceMetadata> searchByKeyword(const std::string& keyword) {
        std::vector<ResourceMetadata> result;
        std::string lowerKeyword = keyword;
        std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
        
        auto it = invertedIndex.find(lowerKeyword);
        if (it != invertedIndex.end()) {
            for (const auto& filename : it->second) {
                auto resIt = filenameIndex.find(filename);
                if (resIt != filenameIndex.end()) {
                    result.push_back(resIt->second);
                }
            }
        }
        
        return result;
    }
    
    std::vector<ResourceMetadata> getResourcesByTag(const std::string& tag) {
        std::vector<ResourceMetadata> result;
        auto it = tagIndex.find(tag);
        if (it != tagIndex.end()) {
            for (const auto& filename : it->second) {
                auto resIt = filenameIndex.find(filename);
                if (resIt != filenameIndex.end()) {
                    result.push_back(resIt->second);
                }
            }
        }
        return result;
    }
    
    std::vector<ResourceMetadata> getResourcesByUploader(const std::string& uploader) {
        std::vector<ResourceMetadata> result;
        auto it = uploaderIndex.find(uploader);
        if (it != uploaderIndex.end()) {
            for (const auto& filename : it->second) {
                auto resIt = filenameIndex.find(filename);
                if (resIt != filenameIndex.end()) {
                    result.push_back(resIt->second);
                }
            }
        }
        return result;
    }
    
    void addResourceRelationship(const std::string& resource1, const std::string& resource2) {
        resourceGraph.addEdge(resource1, resource2);
    }
    
    std::vector<std::string> getRelatedResources(const std::string& resourceFilename) {
        return resourceGraph.getConnected(resourceFilename);
    }
    
    void incrementDownloadCount(const std::string& filename) {
        auto it = filenameIndex.find(filename);
        if (it != filenameIndex.end()) {
            it->second.downloadCount++;
            // Re-add to priority queue with updated count
            popularResources.push(it->second);
        }
    }
    
    std::optional<ResourceMetadata> getResource(const std::string& filename) {
        auto it = filenameIndex.find(filename);
        if (it != filenameIndex.end()) {
            return it->second;
        }
        return std::nullopt;
    }
};

}