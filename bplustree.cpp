#include "bplustree.h"
#include <vector>
#include <algorithm>
using namespace std;

Data::Data(int key, float value) {
    this->key = key;
    this->value = value;
}

Data::~Data() {

}

IntNode::IntNode() {
    this->isLeaf = false;
}

IntNode::~IntNode() {

}

Data* IntNode::search(int key) {
    for (int i=0; i<this->keys.size(); ++i){
        if (key < this->keys[i]){
            return this->children[i].search(key);
        }
    }
    return this->children.back().search(key);
}

void IntNode::insert(int newKey, BPlusTreeNode* newChild) {

    // Overflow split.
    if (this->keys.size() > degree){
        this->split();
    }
}

IntNode* IntNode::split() {
    int midPoint = this->degree / 2;
    auto newNode = new(IntNode);
    for (int i=0; i<midPoint; ++i){
        newNode->keys.push_back(*this->keys.end());
        this->keys.pop_back();
        newNode->children.push_back(*this->children.end());
        this->children.pop_back();
    }
    newNode->children.push_back(*this->children.end());
    this->children.pop_back();
    this->parent->insert(this->keys[midPoint], newNode);
    return newNode;
}

LeafNode::LeafNode() {
    this->isLeaf = true;
}

LeafNode::~LeafNode() {

}

Data* LeafNode::search(int key) {
    for (int i=0; i<this->children.size(); ++i){
        if (this->children[i].key == key){
            return &this->children[i];
        }
    }
    return nullptr;
}

void LeafNode::insert(int newKey, Data* newData) {
    if (this->keys.size() > degree){
        this->split();
    }
}

LeafNode* LeafNode::split() {
    int midPoint = this->degree / 2;
    auto newNode = new(LeafNode);
    for (int i=0; i<midPoint; ++i){
        newNode->keys.push_back(*this->keys.end());
        this->keys.pop_back();
        newNode->children.push_back((*this->children.end()));
        this->children.pop_back();
    }
    newNode->keys.push_back(*this->keys.end());
    this->keys.pop_back();
    this->parent->insert(newNode->children[0].key, newNode);
    return newNode;
}