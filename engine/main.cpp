
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
    Btree<int> tree(3);

    tree.insert(30, 3);
    tree.insert(10, 1);
    tree.insert(20, 2);

    auto rootNames = tree.getRootNames();

    // SprawdŸ czy klucze s¹ obecne (nie testujemy dok³adnej kolejnoœci,
    // poniewa¿ mo¿e siê ró¿niæ w zale¿noœci od implementacji)
    bool found10 = false, found20 = false, found30 = false;
    for (const auto& pair : rootNames) {
        if (pair.first == 10) {
            found10 = true;
            //EXPECT_EQ(pair.second[0], 1);
        }
        else if (pair.first == 20) {
            found20 = true;
            //EXPECT_EQ(pair.second[0], 2);
        }
        else if (pair.first == 30) {
            found30 = true;
            //EXPECT_EQ(pair.second[0], 3);
        }
    }
}
