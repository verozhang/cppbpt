#include <iostream>
#include <fstream>
#include <vector>
#include "bplustree.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout<<"Wrong filename input"<<endl;
        return -1;
    }
    string fileName = argv[1];
    //string fileName = "test";
    ifstream inFile;
    ofstream outFile;
    inFile.open(fileName, ios::in);
    outFile.open("output_file.txt", ios::out);
    string line;
    auto newTree = new(BPlusTree);
    while (getline(inFile, line)) {
        if (!line.compare(0, 10, "Initialize")) {
            int lPos = line.find('(');
            int rPos = line.find(')');
            newTree->degree = stoi(line.substr(lPos + 1, rPos - lPos - 1));
            newTree->root->degree = newTree->degree;
        }
        else if (!line.compare(0, 6, "Insert")) {
            int lPos = line.find('(');
            int mPos = line.find(',');
            int rPos = line.find(')');
            int newKey = stoi(line.substr(lPos + 1, mPos - lPos - 1));
            float newValue = stof(line.substr(mPos + 1, rPos - mPos - 1));
            auto newPair = new(Pair);
            newPair->key = newKey;
            newPair->value = newValue;
            newTree->insert(newPair);
        }
        else if (!line.compare(0, 6, "Delete")) {
            int lPos = line.find('(');
            int rPos = line.find(')');
            int delKey = stoi(line.substr(lPos + 1, rPos - lPos - 1));
            newTree->del(delKey);
        }
        else if (!line.compare(0, 6, "Search")) {
            int lPos = line.find('(');
            int mPos = line.find(',');
            int rPos = line.find(')');
            //Single search
            if (mPos == -1) {
                int searchKey = stoi(line.substr(lPos + 1, rPos - lPos - 1));
                Pair* searchPair = newTree->search(searchKey);
                if (searchPair == nullptr) {
                    outFile<<"Null"<<endl;
                }
                else {
                    outFile<<searchPair->value<<endl;
                }
            }
            //Range search
            else {
                int searchLKey = stoi(line.substr(lPos + 1, mPos - lPos - 1));
                int searchRKey = stoi(line.substr(mPos + 1, rPos - mPos - 1));
                deque<Pair*>* searchDeque = newTree->rangeSearch(searchLKey, searchRKey);
                if (searchDeque->empty()) {
                    outFile<<"Null"<<endl;
                }
                else {
                    while (!searchDeque->empty()) {
                        Pair* curPair = searchDeque->front();
                        outFile<<curPair->value<<',';
                        searchDeque->pop_front();
                    }
                    outFile<<endl;
                }
            }
        }

    }
    inFile.close();
    outFile.close();
}
