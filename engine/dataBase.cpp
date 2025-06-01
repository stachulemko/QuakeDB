#include "dataBase.h"
#include "fileOperationManager.h"

Database::Database() {
    //path = "dataBaseMemoryManagment";
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

void Database::showFile(std::string tableName) {
    std::string filePath = path + "/" + tableName + ".bin";
    std::ifstream file(filePath, std::ios::binary);

    if (!file) {
        std::cerr << "B³¹d: Nie mo¿na otworzyæ pliku " << filePath << std::endl;
        return;
    }
    file.seekg(0, std::ios::end);
    size_t fileSize = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);

    if (fileSize == 0) {
        std::cout << "Plik " << filePath << " jest pusty." << std::endl;
        file.close();
        return;
    }


    std::cout << "=== Plik: " << tableName << ".bin ===" << std::endl;
    std::cout << "Rozmiar: " << fileSize << " bajtów" << std::endl;

    std::vector<uint8_t> buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    std::cout << "Zawartoœæ w bajtach:" << std::endl;
    for (size_t i = 0; i < fileSize; ++i) {
        std::cout << static_cast<int>(buffer[i]) << " ";
        if ((i + 1) % 20 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    file.close();

    for (auto& t : tables) {
        if (t->getTableName() == tableName) {
            std::cout << "\nStruktura tabeli:" << std::endl;
            t->showTable();
            return;
        }
    }

    std::cout << "Tabela " << tableName << " nie jest wczytana do pamiêci." << std::endl;
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
