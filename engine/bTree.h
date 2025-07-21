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
            bool finded = false;
            for (size_t i = 0; i < tmp->getNames().size(); ++i) {
                if (isEqual(data, tmp->getNames()[i].first)) {
                    //path.push_back(tmp);
                    finded = true;
                    break;
                }
                if (!isGreater(data, tmp->getNames()[i].first)) {
                    path.push_back(tmp);
                    tmp = tmp->getChildrens()[i];
                    moved = true;
                    break;
                }
            }
            if (finded) {
                break;
            }
            if (!moved) {
                path.push_back(tmp);
                tmp = tmp->getChildrens()[tmp->getChildrens().size() - 1];
            }
            
        }
        //if()

        path.push_back(tmp);
        tmp->insert(data, blocNum, path, root);
    }

    std::vector<std::pair<T, std::vector<int>>> getRootNames() {
        return root->getNames();
    }

    BtreeNode<T>* getRoot() {
        return root;
    }

    std::vector<int> getBlockNum(T val) {
        std::vector<int> blockNum;
        BtreeNode<T>* tmp = root;

        while (!tmp->getChildrens().empty()) {
            bool moved = false;

            // Sprawdź klucze w bieżącym węźle
            for (size_t i = 0; i < tmp->getNames().size(); i++) {
                // Sprawdź czy klucz jest równy szukanej wartości
                if (isEqual(val, tmp->getNames()[i].first)) {
                    // Dodaj wszystkie numery bloków dla tego klucza
                    for (const auto& block : tmp->getNames()[i].second) {
                        blockNum.push_back(block);
                    }
                }

                // Przejdź do odpowiedniego dziecka, jeśli wartość jest mniejsza
                if (!isGreater(val, tmp->getNames()[i].first) && !isEqual(val, tmp->getNames()[i].first)) {
                    tmp = tmp->getChildrens()[i];
                    moved = true;
                    break;
                }
            }

            // Jeśli nie przesunięto się do żadnego dziecka, idź do ostatniego dziecka
            if (!moved) {
                tmp = tmp->getChildrens()[tmp->getChildrens().size() - 1];
            }
        }

        // Sprawdź klucze w liściu
        for (size_t i = 0; i < tmp->getNames().size(); i++) {
            if (isEqual(val, tmp->getNames()[i].first)) {
                for (int j = 0; j < tmp->getNames()[i].second.size(); j++) {
                    blockNum.push_back(tmp->getNames()[i].second[j]);
                }
            }
        }

        return blockNum;
    }

    void printTree(BtreeNode<T>* node, const std::string& prefix = "", bool isLast = true) {
        if (node == nullptr) return;

        // Wyświetl węzeł
        std::cout << prefix;
        std::cout << (isLast ? "└── " : "├── ");

        // Wyświetl zawartość węzła
        std::cout << "[";
        for (const auto& name : node->getNames()) {
            std::cout << " " << name.first;
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
