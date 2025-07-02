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
	int t;
public:
	BtreeManager(int t, std::vector<std::pair<T, int>>names, std::string tableName) {
		this->t = t;
		bTree = new Btree<T>(t);
		createFolder("bTreeData");
		createBinFile("bTreeData", tableName);
		for (int i = 0; i < names.size(); i++) {
			for (int j = 0; j < names[i].size(); j++) {
				bTree->insert(names[i][j], i);
			}
		}
	}

	int getBlockNum(T val) {
		return bTree->getBlockNum(val);
	}








};








#endif