#ifndef BTREE_H  
#define BTREE_H 

#include <vector>
#include <iostream>
#include <string> 
#include <algorithm>
#include "bTreeNode.h"

template<typename T>
bool isGreater(const T& a, const T& b);

template<typename T>
bool isEqual(const T& a, const T& b);

template <typename T>
class Btree {
private:
    BtreeNode<T>* root;
    int t;

public:
    Btree(int t);

    void insert(const T& data, int blocNum);

    std::vector<std::pair<T, std::vector<int>>> getRootNames();

    BtreeNode<T>* getRoot();

    std::vector<int> getBlockNum(T val);

    void printTree(BtreeNode<T>* node, const std::string& prefix = "", bool isLast = true);

    /*
    std::vector<uint8_t> marshallBtree() {
        std::vector<uint8_t>all;
        std::vector<BtreeNode<T>*>btreeNodes;
        btreeNodes.push_back(*root);
        std::vector<BtreeNode<T>*>childrens = root->getChildrens();
        while (childrens.size() > 0) {
            std::vector<BtreeNode<T>*>childrensNext;
            for (int i = 0; i < childrens.size(); i++) {
                std::vector<BtreeNode<T>>nodeChildrensNext;
                nodeChildrensNext = childrens[i]->getChildrens();
                for (int j = 0; j < nodeChildrensNext.size(); j++) {
                    childrensNext.push_back(nodeChildrensNext[j]);
                }
            }
            for (int i = 0; i < childrens.size(); i++) {
                btreeNodes.push_back(childrens[i]);
            }
            childrens = childrensNext;
        }
        int32_t tmpSize = 0;
        for (int i = 0; i < btreeNodes.size(); i++) {
            tmpSize += btreeNodes[i]->getBtreeNodeSize();
            std::vector<uint8_t>bin = btreeNodes[i]->marshall;
            all.insert(all.end(), bin.begin(), bin.end());
        }
		std::vector<uint8_t>* type = marshalInt32_t(btreeId);
		std::vector<uint8_t>* size = marshalInt32_t(tmpSize);
        all.insert(all.begin()  size->begin(), size->end());
		all.insert(all.begin(), type->begin(), type->end());
        return all;
    }

    void loadBtree(std::vector<uint8_t> bytes) {
        int32_t tmpIndex = 0;
        while (tmpIndex < bytes.size()) {
            if (bytes.size() >= 4) {
                std::vector<uint8_t> typeBytes(bytes->begin()+ tmpIndex, bytes->begin()+ tmpIndex + 4);
                int32_t* type = new int32_t();
                UnmarshalInt32_t(type, &typeBytes);
                if (*type == btreeId) {
                    if (bytes.size() >= 8) {
                        std::vector<uint8_t> sizeBytes(bytes->begin() + tmpIndex + 4, bytes->begin() + tmpIndex+ 8);
                        int32_t* size = new int32_t();
                        UnmarshalInt32_t(size, &sizeBytes);
                        int32_t tmpIndex2 = 0;
                        tmpIndex += 8;
						while (tmpIndex2 < *size) {
							if (bytes.size() >= tmpIndex + 8) {
								std::vector<uint8_t> nodeTypeBytes(bytes->begin() + tmpIndex, bytes->begin() + tmpIndex + 4);
								int32_t* nodeType = new int32_t();
								UnmarshalInt32_t(nodeType, &nodeTypeBytes);
								if (*nodeType == btreeNodeId) {
									//BtreeNode<T>* newNode = new BtreeNode<T>(t);
									std::vector<uint8_t> sizeBytes(bytes->begin() + tmpIndex+4, bytes->begin() + tmpIndex + 8);
                                    int32_t* size = new int32_t();
                                    UnmarshalInt32_t(size, &sizeBytes);
                                    int32_t tmpIndex3 = 0;
                                    while (tmpIndex3 < *size) {
                                        tmpIndex += 8;
                                        if (bytes.size() >= tmpIndex + 8) {
                                            std::vector<uint8_t> stringTypeBytes(bytes->begin() + tmpIndex, bytes->begin() + tmpIndex + 4);
                                            int32_t* nameType = new int32_t();
                                            UnmarshalInt32_t(nameType, &stringTypeBytes);
                                            if (nameType == stringId) {
												std::vector<uint8_t> nameSizeBytes(bytes->begin() + tmpIndex + 4, bytes->begin() + tmpIndex + 8);
												int32_t* nameSize = new int32_t();
												UnmarshalInt32_t(nameSize, &nameSizeBytes);
												if (bytes.size() >= tmpIndex + 8 + *nameSize) {
													//std::vector<uint8_t> nameBytes(bytes->begin() + tmpIndex + 8, bytes->begin() + tmpIndex + 8 + *nameSize);
													//std::string name;
													//UnmarshalString(&name, &nameBytes);
                                                    Tlv name(bytes->begin() + tmpIndex, bytes->begin() + tmpIndex + 8 + *nameSize);
													name.decode();
                                                    name.getValue();
                                                    std::vector<uint8_t> nameVal(bytes->begin() + tmpIndex, bytes->begin() + tmpIndex + 8 + *nameSize);
													//newNode->addName(name, {});
													tmpIndex += 8 + *nameSize;
												}
											}
                                        }
                                    }
									//newNode->loadAllConnectedBytes(nodeBytes);
									//newNode->decode();
									//root = newNode;
									tmpIndex += *size;
									tmpIndex2 += *size;
								}
							}
						}
                    }
                }
            }
        }
        
        
    }
    */
    
};


#include "bTree.cpp"

#endif
