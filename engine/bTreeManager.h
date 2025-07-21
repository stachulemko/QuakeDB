#ifndef BTREEMANAGER_H  
#define BTREEMANAGER_H 
#include <iostream>
#include <vector>
#include "bTree.h"
#include "typeManager.h"
#include "fileOperationManager.h"

template <typename T>
class BtreeManager {
private:
	Btree<T>* bTree;
	int columnIndex = 0;
	int t;
public:
	BtreeManager(int t, std::string tableName,int columnIndex) {
		this->columnIndex = columnIndex;
		this->t = t;
		bTree = new Btree<T>(t);
		createFolder("bTreeData");
		createBinFile("bTreeData", tableName);
	}
	void insert(const T& data, int blockNum) {
		bTree->insert(data, blockNum);
		//std::vector<uint8_t> bytes = bTree->getRoot()->MarshalNode();
		//addToFileBytes("bTreeData/" + std::to_string(t) + ".bin", bytes);
	}

	//int getBlockNum(T val) {
	//	//return bTree->getBlockNum(val);
	//}

	int getColumnIndex() {
		return columnIndex;
	}








};








#endif