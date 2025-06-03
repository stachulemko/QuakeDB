#include "dataBase.h"
#include <filesystem>  

Database::Database() {
    //path = "dataBaseMemoryManagment";
    //std::string tmpPath = executionFilePath() + "/" + path;
	//std::cout << "tmpPath: " << tmpPath << std::endl;
    createFolder(path);
}

Database::~Database() {
    clearAll();
}

void Database::addTable(std::string tableName) {
    Table* newTable = new Table(tableName, path);
    tables.push_back(newTable);
}

void Database::addColumn(std::string tableName, std::string columnName, int dataType, bool allowNull) {
    for (int i = 0; i < tables.size(); i++) {
        if (tables[i]->getTableName() == tableName) {
            tables[i]->addColumn(columnName, dataType, allowNull);
        }
    }
}

void Database::clearAll() {
    for (auto table : tables) {
        table->clearAll();
        delete table;
    }
}

void Database::showFile() {
    for (int i = 0; i < tables.size(); i++) {
        tables[i]->showTable();
    }
}

void Database::commit() {

    for (auto& table : tables) {
        deleteFile(path + "/" + table->getTableName() + ".bin");
		createBinFile(path, table->getTableName());
        std::vector<uint8_t> vec;
        std::vector<uint8_t> columnDefinition = table->getColumnDefinition();
        vec.insert(vec.end(), columnDefinition.begin(), columnDefinition.end());
        std::string tableName = table->getTableName();
        addToFileBytes(path + "/" + tableName + ".bin", vec);
    }

}
void Database::loadDataBase() {
    const std::string folder = path;
	showFileBytes(folder + "/klienci.bin");
	std::cout << "--------------------------" << std::endl;
    try {
        for (const auto& entry : fs::directory_iterator(folder)) {
			if (!fs::is_directory(entry.status())) {
				std::string fileName = entry.path().filename().string();
				Table* table = new Table(fileName.substr(0, fileName.find_last_of('.')), folder);
				std::vector<uint8_t> fileBytes = readFileBytes(entry.path().string());
                table->LoadColumnsDefinition(fileBytes);
                tables.push_back(table);
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
std::vector<std::vector<std::string>> Database::getTableColumnsNames() {
    std::vector<std::vector<std::string>>tablesColumnNames;
    for (int i = 0; i < tables.size(); i++) {
        tablesColumnNames.push_back(tables[i]->getColumnName());
    }
    return tablesColumnNames;
}

std::vector<std::vector<std::vector<int32_t>>> Database::getTypeAndAllowNUllTables() {
	std::vector<std::vector<std::vector<int32_t>>> typeAndAllowNullTables;
	for (int i = 0; i < tables.size(); i++) {
		typeAndAllowNullTables.push_back(tables[i]->getTypeAndAllowNUll());
	}
	return typeAndAllowNullTables;
}
