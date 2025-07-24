#ifndef BTREEMANAGER_H  
#define BTREEMANAGER_H 

#include <iostream>
#include <vector>
#include <string>
#include "bTree.h"
#include "typeManager.h"
#include "fileOperationManager.h"

template <typename T>
class BtreeManager {
private:
    Btree<T>* bTree;
    int columnIndex;
    int t;

public:
    BtreeManager(int t, std::string tableName, int columnIndex);
    void insert(const T& data, int blockNum);
    std::vector<int> getBlockNum(T val);
    int getColumnIndex();
};

#include "bTreeManager.cpp"

#endif
