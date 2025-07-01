
#include "column.h"
#include "tlv.h"
#include "dataBase.h"
int main() {
    std::string tableName = "manyColumnsTable";
    const int numColumns = 100; // Du¿a liczba kolumn

    Database db;
    db.addTable(tableName);

    // Dodaj du¿¹ liczbê kolumn
    for (int i = 0; i < numColumns; i++) {
        std::string colName = "col" + std::to_string(i);
        int dataType = (i % 3) + 1; // Typy 1, 2, 3 rotacyjnie
        bool allowNull = (i % 2 == 0); // Naprzemiennie true/false
		std::cout << "colName : " << colName << "data type : " << dataType << " allowNull : " << allowNull << std::endl;
        db.addColumn(tableName, colName, dataType, allowNull);
    }

    db.commit();

    // £adujemy bazê danych ponownie
    Database loadDb;
    loadDb.loadDataBase();

    // Sprawdzamy czy wszystkie kolumny zosta³y wczytane
    std::vector<std::vector<std::string>> tableColumns = loadDb.getTableColumnsNames();
}
