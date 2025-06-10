#include "dataBase.h"
#include <filesystem>  

Database::Database() {
    createFolder(path);
}

Database::~Database() {
    clearAll();
}

void Database::commit() {
    for (auto& table : tables) {
        deleteFile(path + "/" + table->getTableName() + ".bin");
        createBinFile(path, table->getTableName());
        std::vector<uint8_t> vec;
        std::vector<uint8_t> columnDefinition = table->getColumnDefinition();
        std::vector<uint8_t> recordDefinition = table->getRecordDefinition();
        vec.insert(vec.end(), columnDefinition.begin(), columnDefinition.end());
        vec.insert(vec.end(), recordDefinition.begin(), recordDefinition.end());
        std::string tableName = table->getTableName();
        addToFileBytes(path + "/" + tableName + ".bin", vec);
    }
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

void Database::addRecord(std::string tableName, std::vector<allVars> record) {
    for (int i = 0; i < tables.size(); i++) {
        if (tables[i]->getTableName() == tableName) {
            tables[i]->addRecord(record);
        }
    }
}

void Database::clearAll() {
    for (auto table : tables) {
        table->clearAll();
        delete table;
    }
    tables.clear();
}

int Database::getTableSize() {
    return tables.size();
}

void Database::showFile() {
    for (int i = 0; i < tables.size(); i++) {
        tables[i]->showTable();
    }
}

Database &Database::select(std::string tableName, std::vector<std::string> columnNames) {
    //showSelect(tables, tableName, columnNames);
	selectAcomplished = true;
    sqlQueryBytes.clear();
    AllTableBytes.clear();
    for (int i = 0; i < tables.size(); i++) {
        if (tables[i]->getTableName() == tableName) {
            AllTableBytes = tables[i]->getTableDefinition();
            break;
        }
    }
    sqlQueryBytes = selectFunc(tables, tableName, columnNames);
	return *this;
}

Database &Database::where(std::string columnName, std::string mathOperator, allVars ifValue) {
	if (selectAcomplished) {
        sqlQueryBytes = whereFunc(AllTableBytes,sqlQueryBytes, columnName, mathOperator, ifValue);
	}
	else 
    {
		std::cerr << "Select must be called before where." << std::endl;
	}
	return *this;
}

Database& Database::showSqQuery() {
    if (selectAcomplished) {
        std::vector<std::vector<allVars>> selectedData = sqlQueryBytes;
        if (selectedData.empty()) {
            std::cerr << "No data found for the specified table and columns." << std::endl;
            return *this;
        }
        for (int i = 0; i < selectedData[0].size(); i++) {
            std::cout << "|  ";
            showVariantVariable(selectedData[0][i]);
            std::cout << "  |";

        }
        std::cout << std::endl;
        for (size_t i = 1; i < selectedData.size(); ++i) {
            for (const auto& value : selectedData[i]) {
                std::cout << "|  ";
                //std::cout << "val : " << value << std::endl;
                showVariantVariable(value);
                std::cout << "  |";
            }
            std::cout << std::endl;
		}
	}
    else {
        std::cerr << "Select must be called before showing query." << std::endl;
    }
    clearQueryVariables();
    return *this;
}

void Database::showRecords() {
    for (int i = 0; i < tables.size(); i++) {
        std::cout << "Table: " << tables[i]->getTableName() << std::endl;
        std::vector<std::string> columnNames = tables[i]->getColumnName();
        for (const auto& colName : columnNames) {
            std::cout << colName << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------records-------------" << std::endl;
    for (int i = 0; i < tables.size(); i++) {
        tables[i]->showRecords();
    }
}

std::vector<std::vector<std::string>> Database::getTableColumnsNames() {
    std::vector<std::vector<std::string>> tablesColumnNames;
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

void Database::loadDataBase() {
    const std::string folder = path;
    try {
        for (const auto& entry : fs::directory_iterator(folder)) {
            if (!fs::is_directory(entry.status())) {
                std::string fileName = entry.path().filename().string();
                Table* table = new Table(fileName.substr(0, fileName.find_last_of('.')), folder);
                std::vector<uint8_t> fileBytes = readFileBytes(entry.path().string());
                table->LoadColumnsDefinition(fileBytes);
                table->LoadRecordDefinition(fileBytes);
                tables.push_back(table);
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::string Database::getPath() {
    return path;
}
