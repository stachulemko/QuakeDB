#include "pch.h"
#include "dataBase.h"

/*
void ResetStdoutCapture() {
    testing::internal::GetCapturedStdout();
}
*/
TEST(DataBaseTests, CreateAndDeleteSingleTable) {
    Database db;
    std::string tableName = "testTable";
    db.addTable(tableName);
    db.commit();
    EXPECT_TRUE(isFileExists(db.getPath() + "/" + tableName + ".bin"));
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

TEST(DataBaseTests, AddColumnsAndDelete) {
    Database db;
    std::string tableName = "tableWithColumns";
    db.addTable(tableName);
    db.addColumn(tableName, "col1", 1, false);
    db.addColumn(tableName, "col2", 2, true);
    db.commit();
    EXPECT_TRUE(isFileExists(db.getPath() + "/" + tableName + ".bin"));
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}

TEST(DataBaseTests, MultipleTablesCleanup) {
    Database db;
    std::string t1 = "tab1", t2 = "tab2";
    db.addTable(t1);
    db.addTable(t2);
    db.commit();
    EXPECT_TRUE(isFileExists(db.getPath() + "/" + t1 + ".bin"));
    EXPECT_TRUE(isFileExists(db.getPath() + "/" + t2 + ".bin"));
    deleteFile(db.getPath() + "/" + t1 + ".bin");
    deleteFile(db.getPath() + "/" + t2 + ".bin");
}

TEST(DataBaseTests, CommitAndReloadCleanup) {
    std::string tableName = "reloadTable";
    {
        Database db;
        db.addTable(tableName);
        db.addColumn(tableName, "id", 1, false);
        db.commit();
    }
    Database db2;
    db2.loadDataBase();
    EXPECT_TRUE(isFileExists(db2.getPath() + "/" + tableName + ".bin"));
    deleteFile(db2.getPath() + "/" + tableName + ".bin");
}

TEST(DataBaseTests, SpecialCharactersTableCleanup) {
    std::string tableName = "tab!@#";
    Database db;
    db.addTable(tableName);
    db.commit();
    EXPECT_TRUE(isFileExists(db.getPath() + "/" + tableName + ".bin"));
    deleteFile(db.getPath() + "/" + tableName + ".bin");
}
TEST(DataBaseTests, LongNamesHandling) {
    std::string longTableName(50, 'a');
    std::string longColumnName(50, 'b');
    int type = 1;
    bool allowNUll = false;
    Database db;
    db.addTable(longTableName);
    db.addColumn(longTableName, longColumnName, type, allowNUll);
    db.commit();

    Database loadDb;
    loadDb.loadDataBase();
    if (longColumnName == loadDb.getTableColumnsNames()[0][0]) {
        SUCCEED();
    }
    else {
        FAIL();
    }
	deleteFile(db.getPath() + "/" + longTableName + ".bin");
}
//=================================================

TEST(DataBaseTests, MultipleTablesAndColumnsHandling) {
    std::string longTableName1(50, 'a');
    std::string longColumnName1(50, 'b');
    std::string longColumnName2(50, 'c');
    std::string longTableName2(50, 'd');
    std::string longColumnName3(50, 'e');
    std::string longColumnName4(50, 'f');
    int type1 = 1;
    bool allowNUll1 = false;
    int type2 = 1;
    bool allowNUll2 = false;
    int type3 = 1;
    bool allowNUll3 = false;
    int type4 = 1;
    bool allowNUll4 = false;
    Database db;
    db.addTable(longTableName1);
    db.addColumn(longTableName1, longColumnName1, type1, allowNUll1);
    db.addColumn(longTableName1, longColumnName2, type2, allowNUll2);
    db.addTable(longTableName2);
    db.addColumn(longTableName2, longColumnName3, type3, allowNUll3);
    db.addColumn(longTableName2, longColumnName4, type4, allowNUll4);
    db.commit();

    Database loadDb;
    loadDb.loadDataBase();
    if (loadDb.getTableColumnsNames()[0][0] == longColumnName1) {
        SUCCEED();
    }
    else {
        FAIL();
    }
    
    if (loadDb.getTableColumnsNames()[0][1] == longColumnName2) {
        SUCCEED();
    }
    else {
        FAIL();
    }

    if (loadDb.getTableColumnsNames()[1][0] == longColumnName3) {
        SUCCEED();
    }
    else {
        FAIL();
    }
    if (loadDb.getTableColumnsNames()[1][1] == longColumnName4) {
        SUCCEED();
    }
    else {
        FAIL();
    }
	std::vector<std::vector<std::vector<int32_t>>> typesAndNulls = loadDb.getTypeAndAllowNUllTables();
    for (int i = 0; i < typesAndNulls.size(); i++) {
        if (typesAndNulls[i][0][0] == type1) {
			SUCCEED();
		}
        else {
            FAIL();
        }
        if (typesAndNulls[i][0][1] == 0) {
            SUCCEED();
        }
        else {
			FAIL();
        }
    }
	deleteFile(db.getPath() + "/" + longTableName1 + ".bin");
	deleteFile(db.getPath() + "/" + longTableName2 + ".bin");
}
