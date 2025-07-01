#ifndef BTREENODE_H  
#define BTREENODE_H 

#include <vector>
#include <iostream>
#include <string> 
#include <algorithm>


template <typename T>
class BtreeNode {
private:
    // int - block num , VarType - data
    std::vector<std::pair<int, T>> names;
    std::vector<BtreeNode<T>*> childrens;
    int t;

public:
    BtreeNode(int t) {
        this->t = t;
    }

    std::vector<std::pair<int, T>> getNames() {
        return names;
    }

    void replaceChild(int i, BtreeNode<T>* child) {
        if (i < childrens.size()) {
            childrens[i] = child;
        }
        else {
            std::cout << "Index out of bounds" << std::endl;
        }
    }

    std::vector<BtreeNode<T>*> getChildrens() {
        return childrens;
    }

    T getName() {
        return names[0].second;
    }

    void addChildAtPos(int i, BtreeNode<T>* child) {
        childrens.insert(childrens.begin() + i, child);
    }

    void addChild(BtreeNode<T>* child) {
        childrens.push_back(child);
    }

    void addName(T name, int dataBlock) {
        names.push_back({ dataBlock, name });
        std::sort(names.begin(), names.end(), [](const std::pair<int, T>& a, const std::pair<int, T>& b) {
            return a.second < b.second;
            });
    }

    void insert(T data, int blockNum, std::vector<BtreeNode<T>*> path, BtreeNode<T>*& root) {
        traverse(path, root, data, blockNum);
    }

    std::pair<int, T> splitNode(BtreeNode<T>* node, std::vector<BtreeNode<T>*> path, BtreeNode<T>*& root) {
        BtreeNode<T>* b1 = new BtreeNode<T>(t);
        BtreeNode<T>* b2 = new BtreeNode<T>(t);
        std::vector<std::pair<int, T>> tmpNames = node->getNames();

        for (size_t i = 0; i < node->getNames().size(); i++) {
            if (i < node->getNames().size() / 2) {
                b1->addName(tmpNames[i].second, tmpNames[i].first);
                std::cout << "Adding to b1: " << tmpNames[i].second << std::endl;
            }
            else if (i > node->getNames().size() / 2) {
                b2->addName(tmpNames[i].second, tmpNames[i].first);
                std::cout << "Adding to b2: " << tmpNames[i].second << std::endl;
            }
        }

        auto tmpChildrens = node->getChildrens();
        if (!tmpChildrens.empty()) {
            for (size_t j = 0; j <= b1->getNames().size(); j++) {
                if (j < tmpChildrens.size()) {
                    b1->addChild(tmpChildrens[j]);
                }
            }

            for (size_t j = b1->getNames().size() + 1; j < tmpChildrens.size(); j++) {
                b2->addChild(tmpChildrens[j]);
            }
        }

        BtreeNode<T>* middle = new BtreeNode<T>(t);
        middle->addName(tmpNames[node->getNames().size() / 2].second, tmpNames[node->getNames().size() / 2].first);
        middle->addChild(b1);
        middle->addChild(b2);

        std::cout << "path.size() : " << static_cast<int>(path.size()) << " " << static_cast<int>(path.size()) - 2 << std::endl;

        if (static_cast<int>(path.size()) - 2 >= 0) {
            for (size_t k = 0; k < path[path.size() - 2]->getChildrens().size(); k++) {
                if (path[path.size() - 2]->getChildrens()[k] == node) {
                    path[path.size() - 2]->replaceChild(k, b1);
                    path[path.size() - 2]->addChildAtPos(k + 1, b2);
                    break;
                }
            }
        }
        else {
            root = middle;
        }

        T middleValue = tmpNames[node->getNames().size() / 2].second;
        int blockData = tmpNames[node->getNames().size() / 2].first;

        return { blockData, middleValue };
    }

    void traverse(std::vector<BtreeNode<T>*> path, BtreeNode<T>*& root, T data, int blockData) {
        if (path.empty()) {
            return;
        }

        path[path.size() - 1]->addName(data, blockData);

        if (path[path.size() - 1]->getNames().size() >= t) {
            std::pair<int, T> newData = splitNode(path[path.size() - 1], path, root);
            path.pop_back();
            if (!path.empty()) {
                traverse(path, root, newData.second, newData.first);
            }
        }
    }
};

#endif
