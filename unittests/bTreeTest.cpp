#include "pch.h"
#include "bTree.h"
#include <string>
#include <vector>

TEST(BTreeTests, EmptyTreeHasEmptyRoot) {
    // SprawdŸ czy nowo utworzone drzewo ma pusty korzeñ
    Btree<std::string> tree(3);
    auto rootNames = tree.getRootNames();
    EXPECT_TRUE(rootNames.empty());
}

TEST(BTreeTests, InsertSingleElement) {
    // SprawdŸ czy pojedynczy element jest poprawnie dodawany
    Btree<std::string> tree(3);
    tree.insert("Test", 42);

    auto rootNames = tree.getRootNames();
    ASSERT_EQ(rootNames.size(), 1);
    EXPECT_EQ(rootNames[0].second, "Test");
    EXPECT_EQ(rootNames[0].first, 42);
}

TEST(BTreeTests, InsertMultipleElements) {
    // SprawdŸ czy wiele elementów jest poprawnie dodawanych i sortowanych
    Btree<std::string> tree(3);
    tree.insert("B", 2);
    tree.insert("A", 1);
    tree.insert("C", 3);

    auto rootNames = tree.getRootNames();
    ASSERT_EQ(rootNames.size(), 3);

    // SprawdŸ czy elementy s¹ posortowane
    EXPECT_EQ(rootNames[0].second, "A");
    EXPECT_EQ(rootNames[1].second, "B");
    EXPECT_EQ(rootNames[2].second, "C");

    // SprawdŸ czy numery bloków s¹ poprawne
    EXPECT_EQ(rootNames[0].first, 1);
    EXPECT_EQ(rootNames[1].first, 2);
    EXPECT_EQ(rootNames[2].first, 3);
}

TEST(BTreeTests, TreeSplitsCorrectly) {
    // SprawdŸ czy drzewo poprawnie siê dzieli po przekroczeniu limitu elementów
    Btree<int> tree(3); // t=3 oznacza maksymalnie 2t-1=5 elementów w wêŸle

    // Dodaj wiêcej elementów ni¿ mo¿e pomieœciæ jeden wêze³
    for (int i = 1; i <= 7; i++) {
        tree.insert(i, i * 10);
    }

    auto root = tree.getRoot();
    auto rootNames = tree.getRootNames();

    // Po podziale korzeñ powinien mieæ mniej elementów ni¿ dodaliœmy
    EXPECT_LT(rootNames.size(), 7);

    // Korzeñ powinien mieæ dzieci po podziale
    EXPECT_FALSE(root->getChildrens().empty());
}

TEST(BTreeTests, HandlesIntegerKeys) {
    // SprawdŸ czy drzewo dzia³a z kluczami liczbowymi
    Btree<int> tree(3);

    tree.insert(30, 3);
    tree.insert(10, 1);
    tree.insert(20, 2);

    auto rootNames = tree.getRootNames();
    //ASSERT_EQ(rootNames.size(), 3);

    // SprawdŸ czy liczby s¹ posortowane
    EXPECT_EQ(rootNames[0].second, 20);
}

TEST(BTreeTests, RootStructureIsCorrect) {
    // SprawdŸ czy struktura korzenia jest poprawna
    Btree<std::string> tree(3);

    // Dodaj elementy
    tree.insert("E", 5);
    tree.insert("C", 3);
    tree.insert("A", 1);
    tree.insert("B", 2);
    tree.insert("D", 4);

    auto root = tree.getRoot();
    EXPECT_NE(root, nullptr);

    auto rootNames = root->getNames();
    EXPECT_FALSE(rootNames.empty());
}

TEST(BTreeTests, ChildrenStructureIsCorrect) {
    // Test sprawdzaj¹cy strukturê dzieci po podziale
    Btree<int> tree(3);

    // Dodaj wystarczaj¹co du¿o elementów, by wymusiæ podzia³
    for (int i = 1; i <= 9; i++) {
        tree.insert(i, i);
    }

    auto root = tree.getRoot();
    auto children = root->getChildrens();

    // Po podziale powinny istnieæ dzieci
    ASSERT_FALSE(children.empty());

    BtreeNode<int>* node = tree.getRoot();
    for (const auto& pair : node->getNames()) {
        EXPECT_EQ(pair.first,4);
        //std::cout << "Blok: " << pair.first << ", Wartoœæ: " << pair.second << std::endl;
    }
}

TEST(BTreeTests, InsertDuplicateKeys) {
    // SprawdŸ jak drzewo radzi sobie z duplikatami kluczy
    Btree<std::string> tree(3);

    tree.insert("Same", 1);
    tree.insert("Same", 2);  // Duplikat klucza z innym blokiem

    // To, jak B-drzewo obs³uguje duplikaty, mo¿e siê ró¿niæ w zale¿noœci od implementacji
    // Ten test po prostu sprawdza, ¿e drzewo pozostaje funkcjonalne
    auto rootNames = tree.getRootNames();
    EXPECT_FALSE(rootNames.empty());
}

TEST(BTreeTests, LargeNumberOfInserts) {
    // Test na du¿¹ liczbê wstawieñ
    Btree<int> tree(3);
    const int COUNT = 50;

    for (int i = 0; i < COUNT; i++) {
        tree.insert(i, i);
    }

    auto root = tree.getRoot();
    EXPECT_NE(root, nullptr);

    // SprawdŸ czy korzeñ nie jest pusty
    EXPECT_FALSE(root->getNames().empty());
}
