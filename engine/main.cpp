
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
    Btree<int> tree(3);

    // Dodaj wystarczaj¹co du¿o elementów, aby utworzyæ z³o¿one drzewo
    for (int i = 0; i < 100; i++) {
        tree.insert(i, i * 10);
    }

    // Dodaj kilka duplikatów kluczy
    std::vector<std::pair<int, int>> duplicates = {
        {25, 2500}, {25, 2501}, {25, 2502},
        {50, 5000}, {50, 5001},
        {75, 7500}
    };

    for (const auto& [key, blockNum] : duplicates) {
        tree.insert(key, blockNum);
    }
    tree.printTree(tree.getRoot());
    // SprawdŸ getBlockNum dla klucza z wieloma wyst¹pieniami
    std::vector<int> blockNums25 = tree.getBlockNum(25);
}
