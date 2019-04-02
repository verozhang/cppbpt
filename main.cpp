#include <iostream>
#include <vector>
#include "bplustree.h"
using namespace std;


int main() {
    auto newTree = new(BPlusTree);
    newTree->degree = 3;
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
    cout<<"Hello world!";
}