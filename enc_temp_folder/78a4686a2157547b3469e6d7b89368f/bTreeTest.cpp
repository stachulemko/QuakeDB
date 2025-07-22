#include "pch.h"
#include "bTree.h"
#include <string>
#include <vector>
#include <algorithm>

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
    EXPECT_EQ(rootNames[0].first, "Test");
    EXPECT_EQ(rootNames[0].second.size(), 1);
    EXPECT_EQ(rootNames[0].second[0], 42);
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
    EXPECT_EQ(rootNames[0].first, "A");
    EXPECT_EQ(rootNames[1].first, "B");
    EXPECT_EQ(rootNames[2].first, "C");

    // SprawdŸ czy numery bloków s¹ poprawne
    EXPECT_EQ(rootNames[0].second[0], 1);
    EXPECT_EQ(rootNames[1].second[0], 2);
    EXPECT_EQ(rootNames[2].second[0], 3);
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
    
    // SprawdŸ czy klucze s¹ obecne (nie testujemy dok³adnej kolejnoœci,
    // poniewa¿ mo¿e siê ró¿niæ w zale¿noœci od implementacji)
    bool found10 = false, found20 = false, found30 = false;
    for (const auto& pair : rootNames) {
        if (pair.first == 10) {
            found10 = true;
            EXPECT_EQ(pair.second[0], 1);
        }
        else if (pair.first == 20) {
            found20 = true;
            EXPECT_EQ(pair.second[0], 2);
        }
        else if (pair.first == 30) {
            found30 = true;
            EXPECT_EQ(pair.second[0], 3);
        }
    }
    
    EXPECT_TRUE(found10 && found20 && found30) << "Nie znaleziono wszystkich kluczy";
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
}

TEST(BTreeTests, InsertDuplicateKeys) {
    // SprawdŸ jak drzewo radzi sobie z duplikatami kluczy
    Btree<std::string> tree(3);

    tree.insert("Same", 1);
    tree.insert("Same", 2);  // Duplikat klucza z innym blokiem

    // To, jak B-drzewo obs³uguje duplikaty, mo¿e siê ró¿niæ w zale¿noœci od implementacji
    // Ten test sprawdza, czy dla klucza "Same" mamy oba numery bloków
    std::vector<int> blockNums = tree.getBlockNum("Same");
    EXPECT_EQ(blockNums.size(), 2);
    
    // SprawdŸ czy oba numery bloków s¹ obecne
    bool found1 = false, found2 = false;
    for (int blockNum : blockNums) {
        if (blockNum == 1) found1 = true;
        if (blockNum == 2) found2 = true;
    }
    
    EXPECT_TRUE(found1 && found2) << "Nie znaleziono wszystkich numerów bloków dla zduplikowanego klucza";
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

// Testy dla funkcji getBlockNum zwracaj¹cej wektor

TEST(BTreeTests, GetBlockNumReturnsEmptyForNonexistentValue) {
    // SprawdŸ, czy getBlockNum zwraca pusty wektor dla nieistniej¹cego elementu
    Btree<std::string> tree(3);
    tree.insert("Test", 42);

    std::vector<int> blockNums = tree.getBlockNum("Nonexistent");
    EXPECT_TRUE(blockNums.empty()) << "getBlockNum powinien zwracaæ pusty wektor dla nieistniej¹cego elementu";
}

TEST(BTreeTests, GetBlockNumReturnsSingleValueForUniqueKey) {
    // SprawdŸ, czy getBlockNum zwraca wektor z jednym elementem dla unikalnego klucza
    Btree<std::string> tree(3);
    tree.insert("Unique", 123);

    std::vector<int> blockNums = tree.getBlockNum("Unique");
    ASSERT_EQ(blockNums.size(), 1) << "getBlockNum powinien zwracaæ wektor z jednym elementem dla unikalnego klucza";
    EXPECT_EQ(blockNums[0], 123) << "Zwrócony numer bloku powinien byæ zgodny z oczekiwanym";
}

TEST(BTreeTests, GetBlockNumReturnsAllOccurrencesOfDuplicateKey) {
    // SprawdŸ, czy getBlockNum zwraca wszystkie wyst¹pienia duplikatu klucza
    Btree<std::string> tree(3);
    tree.insert("Duplicate", 1);
    tree.insert("Duplicate", 2);
    tree.insert("Duplicate", 3);
    tree.insert("Unique", 4);
    tree.insert("Duplicate", 5);

    std::vector<int> blockNums = tree.getBlockNum("Duplicate");

    // SprawdŸ czy znaleziono wszystkie wyst¹pienia
    ASSERT_EQ(blockNums.size(), 4) << "getBlockNum powinien zwróciæ wszystkie 4 wyst¹pienia klucza";

    // SprawdŸ czy wszystkie oczekiwane numery bloków s¹ obecne
    std::vector<int> expectedBlockNums = { 1, 2, 3, 5 };
    for (int expected : expectedBlockNums) {
        bool found = false;
        for (int actual : blockNums) {
            if (actual == expected) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found) << "Numer bloku " << expected << " nie zosta³ znaleziony";
    }
}

TEST(BTreeTests, GetBlockNumAfterSplit) {
    // SprawdŸ, czy getBlockNum dzia³a poprawnie po podziale wêz³a
    Btree<int> tree(3);

    // Dodaj wiêcej elementów ni¿ mo¿e pomieœciæ jeden wêze³
    for (int i = 1; i <= 10; i++) {
        tree.insert(i, i * 10);
    }

    // Dodaj duplikaty kluczy
    tree.insert(5, 500);
    tree.insert(5, 501);

    // SprawdŸ czy wszystkie wyst¹pienia klucza 5 s¹ zwracane
    std::vector<int> blockNums = tree.getBlockNum(5);
    ASSERT_EQ(blockNums.size(), 3) << "getBlockNum powinien znaleŸæ 3 wyst¹pienia klucza 5";

    // SprawdŸ czy wszystkie oczekiwane numery bloków s¹ obecne
    std::vector<int> expectedBlockNums = { 50, 500, 501 };
    for (int expected : expectedBlockNums) {
        bool found = false;
        for (int actual : blockNums) {
            if (actual == expected) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found) << "Numer bloku " << expected << " nie zosta³ znaleziony";
    }
}

TEST(BTreeTests, GetBlockNumFindsValuesInLeafAndNonLeafNodes) {
    // SprawdŸ, czy getBlockNum znajduje wartoœci zarówno w wêz³ach liœci jak i wêz³ach wewnêtrznych
    Btree<int> tree(3);

    // Dodaj elementy, aby utworzyæ drzewo o wysokoœci co najmniej 2
    for (int i = 1; i <= 20; i++) {
        tree.insert(i, i * 10);
    }

    // Dodaj duplikaty kluczy w ró¿nych wêz³ach
    tree.insert(5, 501);   // Ten mo¿e trafiæ do innego wêz³a
    tree.insert(15, 151);  // Ten mo¿e trafiæ do innego wêz³a

    // SprawdŸ dla klucza, który powinien mieæ 2 wyst¹pienia
    std::vector<int> blockNums5 = tree.getBlockNum(5);
    ASSERT_GE(blockNums5.size(), 2) << "getBlockNum powinien znaleŸæ co najmniej 2 wyst¹pienia klucza 5";

    // SprawdŸ czy znaleziono oba numery bloków
    bool found50 = std::find(blockNums5.begin(), blockNums5.end(), 50) != blockNums5.end();
    bool found501 = std::find(blockNums5.begin(), blockNums5.end(), 501) != blockNums5.end();

    EXPECT_TRUE(found50) << "Nie znaleziono numeru bloku 50 dla klucza 5";
    EXPECT_TRUE(found501) << "Nie znaleziono numeru bloku 501 dla klucza 5";

    // SprawdŸ dla innego klucza
    std::vector<int> blockNums15 = tree.getBlockNum(15);
    ASSERT_GE(blockNums15.size(), 2) << "getBlockNum powinien znaleŸæ co najmniej 2 wyst¹pienia klucza 15";

    bool found150 = std::find(blockNums15.begin(), blockNums15.end(), 150) != blockNums15.end();
    bool found151 = std::find(blockNums15.begin(), blockNums15.end(), 151) != blockNums15.end();

    EXPECT_TRUE(found150) << "Nie znaleziono numeru bloku 150 dla klucza 15";
    EXPECT_TRUE(found151) << "Nie znaleziono numeru bloku 151 dla klucza 15";
}

TEST(BTreeTests, GetBlockNumWithManyDuplicatesAcrossNodes) {
    // Test z du¿¹ liczb¹ duplikatów rozproszonych po ca³ym drzewie
    Btree<std::string> tree(3);

    // Dodaj ró¿ne wartoœci i ich duplikaty
    std::string testKey = "TestKey";
    std::vector<int> expectedBlockNums;

    // Dodaj 20 duplikatów
    for (int i = 1; i <= 20; i++) {
        int blockNum = i * 10;
        tree.insert(testKey, blockNum);
        expectedBlockNums.push_back(blockNum);
    }

    // Dodaj inne klucze, aby wymusiæ podzia³y
    for (int i = 1; i <= 50; i++) {
        tree.insert("OtherKey" + std::to_string(i), i);
    }

    // Dodaj jeszcze wiêcej duplikatów testowego klucza
    for (int i = 21; i <= 30; i++) {
        int blockNum = i * 10;
        tree.insert(testKey, blockNum);
        expectedBlockNums.push_back(blockNum);
    }

    // SprawdŸ czy getBlockNum znajduje wszystkie wyst¹pienia
    std::vector<int> foundBlockNums = tree.getBlockNum(testKey);
    ASSERT_EQ(foundBlockNums.size(), 30) << "getBlockNum powinien znaleŸæ wszystkie 30 wyst¹pieñ klucza";

    // SprawdŸ czy wszystkie oczekiwane numery bloków s¹ obecne
    for (int expected : expectedBlockNums) {
        bool found = std::find(foundBlockNums.begin(), foundBlockNums.end(), expected) != foundBlockNums.end();
        EXPECT_TRUE(found) << "Numer bloku " << expected << " nie zosta³ znaleziony";
    }
}

TEST(BTreeTests, GetBlockNumInComplexTree) {
    // Test ze z³o¿onym drzewem o wielu poziomach
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

    // SprawdŸ getBlockNum dla klucza z wieloma wyst¹pieniami
    std::vector<int> blockNums25 = tree.getBlockNum(25);
    ASSERT_EQ(blockNums25.size(), 4) << "getBlockNum powinien znaleŸæ 4 wyst¹pienia klucza 25";

    std::vector<int> expected25 = { 250, 2500, 2501, 2502 };
    for (int expected : expected25) {
        bool found = std::find(blockNums25.begin(), blockNums25.end(), expected) != blockNums25.end();
        EXPECT_TRUE(found) << "Numer bloku " << expected << " nie zosta³ znaleziony dla klucza 25";
    }

    // SprawdŸ dla innego klucza
    std::vector<int> blockNums50 = tree.getBlockNum(50);
    ASSERT_EQ(blockNums50.size(), 3) << "getBlockNum powinien znaleŸæ 3 wyst¹pienia klucza 50";

    std::vector<int> expected50 = { 500, 5000, 5001 };
    for (int expected : expected50) {
        bool found = std::find(blockNums50.begin(), blockNums50.end(), expected) != blockNums50.end();
        EXPECT_TRUE(found) << "Numer bloku " << expected << " nie zosta³ znaleziony dla klucza 50";
    }
}
