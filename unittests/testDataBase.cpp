#pragma once 
#include "pch.h"
#include "dataBase.h"

TEST(DataBaseTests,testFolderCreated) {
	Database database;
	EXPECT_TRUE(isFileExists(database.getPath()));
	
}
TEST(DataBaseTests, fileWasCreated) {
	Database database;
	database.addTable("testTable");
	database.commit();
	EXPECT_TRUE(isFileExists(database.getPath() + "/testTable.bin"));
	//deleteFile(database.getPath() + "/testTable.bin");
}
