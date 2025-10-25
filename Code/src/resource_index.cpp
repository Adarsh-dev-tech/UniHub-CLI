#include "resource_index.h"
#include <algorithm>
#include <sstream>

namespace uni {

// ============================================================================
// B-Tree Implementation
// ============================================================================

void BTreeNode::insertNonFull(const ResourceMetadata& resource) {
    int i = static_cast<int>(keys.size()) - 1;
    
    if (isLeaf) {
        keys.resize(keys.size() + 1);
        while (i >= 0 && resource.filename < keys[i].filename) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = resource;
    } else {
        while (i >= 0 && resource.filename < keys[i].filename) {
            i--;
        }
        i++;
        
        if (children[i]->keys.size() == (2 * MIN_DEGREE) - 1) {
            splitChild(i, children[i]);
            if (resource.filename > keys[i].filename) {
                i++;
            }
        }
        children[i]->insertNonFull(resource);
    }
}

void BTreeNode::splitChild(int index, std::shared_ptr<BTreeNode> child) {
    auto newChild = std::make_shared<BTreeNode>(child->isLeaf);
    
    for (int j = 0; j < MIN_DEGREE - 1; j++) {
        newChild->keys.push_back(child->keys[j + MIN_DEGREE]);
    }
    
    if (!child->isLeaf) {
        for (int j = 0; j < MIN_DEGREE; j++) {
            newChild->children.push_back(child->children[j + MIN_DEGREE]);
        }
        child->children.resize(MIN_DEGREE);
    }
    
    children.insert(children.begin() + index + 1, newChild);
    keys.insert(keys.begin() + index, child->keys[MIN_DEGREE - 1]);
    
    child->keys.resize(MIN_DEGREE - 1);
}

std::vector<ResourceMetadata> BTreeNode::search(const std::string& query) {
    std::vector<ResourceMetadata> results;
    
    for (const auto& resource : keys) {
        if (resource.filename.find(query) != std::string::npos ||
            resource.displayName.find(query) != std::string::npos) {
            results.push_back(resource);
        }
    }
    
    if (!isLeaf) {
        for (auto& child : children) {
            auto childResults = child->search(query);
            results.insert(results.end(), childResults.begin(), childResults.end());
        }
    }
    
    return results;
}

}