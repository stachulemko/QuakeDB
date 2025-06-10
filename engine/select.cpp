#include "select.h"

std::vector<std::vector<allVars>> selectFunc(std::vector<Table*> tables, std::string tableName, std::vector<std::string> columnNames) {
    std::vector<std::vector<allVars>> selectData;
    
    for (int i = 0; i < tables.size(); i++) {
        if (tables[i]->getTableName() == tableName) {
            std::vector<std::string> columns = tables[i]->getColumnName();
            std::vector<int> columnIndexes;
            selectData.push_back({});
            
            if (columnNames[0] != "*") {
                for (const auto& colName : columnNames) {
                    int foundIndex = -1;
                    for (int j = 0; j < columns.size(); j++) {
                        if (columns[j] == colName) {
                            foundIndex = j;
                            break;
                        }
                    }
                    
                    if (foundIndex == -1) {
                        std::cerr << "Error: Column '" << colName << "' does not exist in table '" << tableName << "'." << std::endl;
                        return {};
                    }
                    
                    selectData[0].push_back(colName);
                    columnIndexes.push_back(foundIndex);
                }
                
                std::vector<Record*> selectedColumns = tables[i]->getRecords();
                for (int k = 0; k < selectedColumns.size(); k++) {
                    selectData.push_back({});
                    std::vector<Tlv*> tlvvec = selectedColumns[k]->getRecordDataTlv();
                    
                    for (int j = 0; j < columnIndexes.size(); j++) {
                        selectData[k + 1].push_back(tlvvec[columnIndexes[j]]->getValue());
                    }
                }
            }
            else {
                for (int j = 0; j < columns.size(); j++) {
                    selectData[0].push_back(columns[j]);
                    columnIndexes.push_back(j);
                }
                
                std::vector<Record*> selectedColumns = tables[i]->getRecords();
                for (int k = 0; k < selectedColumns.size(); k++) {
                    selectData.push_back({});
                    std::vector<Tlv*> tlvvec = selectedColumns[k]->getRecordDataTlv();
                    for (int j = 0; j < columnIndexes.size(); j++) {
                        selectData[k + 1].push_back(tlvvec[columnIndexes[j]]->getValue());
                    }
                }
            }
            
            return selectData;  
        }
    }
    
    return selectData;  
}

void showSelect(std::vector<Table*> tables, std::string tableName, std::vector<std::string> columnNames) {
	std::vector<std::vector<allVars>> selectedData = selectFunc(tables, tableName, columnNames);
	if (selectedData.empty()) {
		std::cerr << "No data found for the specified table and columns." << std::endl;
		return;
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