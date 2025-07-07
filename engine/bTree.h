#ifndef BTREE_H  
#define BTREE_H 

#include <vector>
#include <iostream>
#include <string> 
#include <algorithm>
#include "bTreeNode.h"

template<typename T>
bool isGreater(const T& a, const T& b) {
    return a > b;
}

template<typename T>
bool isEqual(const T& a, const T& b) {
	return a == b;
}

template <typename T>
class Btree {
private:
    BtreeNode<T>* root;
    int t;

public:
    Btree(int t) {
        this->t = t;
        root = new BtreeNode<T>(t);
    }

    void insert(const T& data, int blocNum) {
        std::vector<BtreeNode<T>*> path;
        BtreeNode<T>* tmp = root;

        while (!tmp->getChildrens().empty()) {
            bool moved = false;
            for (size_t i = 0; i < tmp->getNames().size(); ++i) {
                if (!isGreater(data, tmp->getNames()[i].second)) {
                    path.push_back(tmp);
                    tmp = tmp->getChildrens()[i];
                    moved = true;
                    break;
                }
            }
            if (!moved) {
                path.push_back(tmp);
                tmp = tmp->getChildrens()[tmp->getChildrens().size() - 1];
            }
        }

        path.push_back(tmp);
        tmp->insert(data, blocNum, path, root);
    }

    std::vector<std::pair<int, T>> getRootNames() {
        return root->getNames();
    }

    BtreeNode<T>* getRoot() {
        return root;
    }


    int getBlockNum(T val) {
        BtreeNode<T>* tmp = root;
        while (!tmp->getChildrens().empty()) {
            bool moved = false;
            for (size_t i = 0; i < tmp->getNames().size(); ++i) {
                if (!isGreater(val, tmp->getNames()[i].second) && !isEqual(val, tmp->getNames()[i].second)) {
                    tmp = tmp->getChildrens()[i];
                    moved = true;
                    break;
                }
                else if (isEqual(val, tmp->getNames()[i].second)) {
					return tmp->getNames()[i].first;
                }
            }
            if (!moved) {
                //path.push_back(tmp);
                tmp = tmp->getChildrens()[tmp->getChildrens().size() - 1];
            }
            /*
            if (!moved) {
                //path.push_back(tmp);
                //tmp = tmp->getChildrens()[tmp->getChildrens().size() - 1];
            }
            */
        }
    }
    void printTree(BtreeNode<T>* node, const std::string& prefix = "", bool isLast = true) {
        if (node == nullptr) return;

        // Wyświetl węzeł
        std::cout << prefix;
        std::cout << (isLast ? "└── " : "├── ");

        // Wyświetl zawartość węzła
        std::cout << "[";
        for (const auto& name : node->getNames()) {
            std::cout << " " << name.second;
        }
        std::cout << " ]" << std::endl;

        // Wyświetl dzieci
        auto children = node->getChildrens();
        for (size_t i = 0; i < children.size(); ++i) {
            std::string newPrefix = prefix + (isLast ? "    " : "│   ");
            bool lastChild = (i == children.size() - 1);
            printTree(children[i], newPrefix, lastChild);
        }
    }
};


#endif

