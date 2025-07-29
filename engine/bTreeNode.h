#ifndef BTREENODE_H  
#define BTREENODE_H 

#include <vector>
#include <iostream>
#include <string> 
#include <algorithm>
#include "typeManager.h"
#include "binaryConverter.h"
#include "tlv.h"

template <typename T>
class BtreeNode {
private:
    // int - klucz, std::vector<int> - numery bloków dla danego klucza
    std::vector<std::pair<T, std::vector<int>>> names;
    std::vector<BtreeNode<T>*> childrens;
    int32_t btreeNodeSize = 0;
    int t;

public:
    BtreeNode(int t);

	int32_t getBtreeNodeSize() {
		return btreeNodeSize;
	}

    std::vector<std::pair<T, std::vector<int>>> getNames();

    void replaceChild(int i, BtreeNode<T>* child);

    std::vector<BtreeNode<T>*> getChildrens();

    T getName();

    void addChildAtPos(int i, BtreeNode<T>* child);

    void addChild(BtreeNode<T>* child);

    void addName(T name, int dataBlock);

    void insert(T data, int blockNum, std::vector<BtreeNode<T>*> path, BtreeNode<T>*& root);

    std::pair<T, std::vector<int>> splitNode(BtreeNode<T>* node, std::vector<BtreeNode<T>*> path, BtreeNode<T>*& root);

    void traverse(std::vector<BtreeNode<T>*> path, BtreeNode<T>*& root, T data, int blockData);

    //std::vector<uint8_t> marshall();

    
};

#include "bTreeNode.cpp"

#endif

