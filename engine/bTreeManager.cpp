#ifndef BTREEMANAGER_CPP
#define BTREEMANAGER_CPP

#include "bTreeManager.h"

template <typename T>
BtreeManager<T>::BtreeManager(int t, std::string tableName, int columnIndex) {
    this->columnIndex = columnIndex;
    this->t = t;
    bTree = new Btree<T>(t);
    createFolder("bTreeData");
    createBinFile("bTreeData", tableName);
}

template <typename T>
void BtreeManager<T>::insert(const T& data, int blockNum) {
    bTree->insert(data, blockNum);
    //std::vector<uint8_t> bytes = bTree->getRoot()->MarshalNode();
    //addToFileBytes("bTreeData/" + std::to_string(t) + ".bin", bytes);
}

template <typename T>
std::vector<int> BtreeManager<T>::getBlockNum(T val) {
    return bTree->getBlockNum(val);
}

template <typename T>
int BtreeManager<T>::getColumnIndex() {
    return columnIndex;
}

#endif
