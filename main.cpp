#include <iostream>
#include <vector>
#include "bplustree.h"
using namespace std;

int main() {
    auto newTree = new(BPlusTree);
    auto newPair = new(Pair);
    newPair->key = 1;
    newPair->value = 3;
    newTree->insert(newPair);
    auto newPair2 = new(Pair);
    newPair2->key = 2;
    newPair2->value = 3;
    newTree->insert(newPair2);
    auto newPair3 = new(Pair);
    newPair3->key = 3;
    newPair3->value = 3;
    newTree->insert(newPair3);
    auto newPair4 = new(Pair);
    newPair4->key = 4;
    newPair4->value = 3;
    newTree->insert(newPair4);
    auto newPair5 = new(Pair);
    newPair5->key = 5;
    newPair5->value = 3;
    newTree->insert(newPair5);
    auto newPair6 = new(Pair);
    newPair6->key = 6;
    newPair6->value = 3;
    newTree->insert(newPair6);
    auto newPair7 = new(Pair);
    newPair7->key = 7;
    newPair7->value = 3;
    newTree->insert(newPair7);
    auto newPair8 = new(Pair);
    newPair8->key = 8;
    newPair8->value = 3;
    newTree->insert(newPair8);
    auto newPair9 = new(Pair);
    newPair9->key = 9;
    newPair9->value = 3;
    newTree->insert(newPair9);
    auto newPair10 = new(Pair);
    newPair10->key = 10;
    newPair10->value = 3;
    newTree->insert(newPair10);
    auto newPair11 = new(Pair);
    newPair11->key = 11;
    newPair11->value = 3;
    newTree->insert(newPair11);

    cout<<"Hello world!";
}