#include <iostream>
#include <vector>
#include "bplustree.h"
using namespace std;


int main() {
    auto newTree = new(BPlusTree);
    auto newPair = new(Pair);
    newPair->key = 3;
    newPair->value = 3;
    newTree->insert(newPair);
    auto newPair2 = new(Pair);
    newPair2->key = 4;
    newPair2->value = 3;
    newTree->insert(newPair2);
    auto newPair3 = new(Pair);
    newPair3->key = 2;
    newPair3->value = 3;
    newTree->insert(newPair3);
    auto newPair4 = new(Pair);
    newPair4->key = 1;
    newPair4->value = 3;
    newTree->insert(newPair4);
    auto newPair5 = new(Pair);
    newPair5->key = 5;
    newPair5->value = 3;
    newTree->insert(newPair5);
    cout<<"Hello world!";
}