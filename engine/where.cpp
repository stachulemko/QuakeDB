#include "where.h"

std::vector<std::vector<allVars>> whereFunc(std::vector<std::vector<allVars>> AllTableData , std::vector<std::vector<allVars>> lastState, std::string columnName, std::string mathOperator, allVars ifValue) {
	std::cout << "-----------------------------------------" << std::endl;
	showVariantVectorOfVector(AllTableData);
	std::cout << "-----------------------------------------" << std::endl;
	std::vector<std::vector<allVars>> result = lastState;
	int32_t columnIndex = -1;
	for (int i = 0; i < AllTableData[0].size(); i++) {
		if (std::holds_alternative<std::string>(AllTableData[0][i]) &&
			std::get<std::string>(AllTableData[0][i]) == columnName) {
			columnIndex = i;
		}
	}
	if (columnIndex == -1) {
		std::cerr << "Error: Column '" << columnName << "' not found in the last state." << std::endl;
		return {};
	}
	else{
		int i = 1;
		std::vector<int>toDelete;
		while(i<AllTableData.size()){
			if (getTypeId(ifValue) != getTypeId(AllTableData[i][columnIndex])) {
				std::cerr << "Error: Type mismatch for column '" << columnName << "'. Expected type: " << getTypeId(result[i][columnIndex]) << ", but got: " << getTypeId(ifValue) << std::endl;
				return {};
			}
			else {
				if (mathOperator == "=") {
					if (AllTableData[i][columnIndex] != ifValue) {
						toDelete.push_back(i);
					}
				}
				else if (mathOperator == "!=") {
					if (AllTableData[i][columnIndex] == ifValue) {
						toDelete.push_back(i);
					}
				}
				else if (mathOperator == "<") {
					if (AllTableData[i][columnIndex] >= ifValue) {
						toDelete.push_back(i);
					}
				}
				else if (mathOperator == ">") {
					if (AllTableData[i][columnIndex] <= ifValue) {
						toDelete.push_back(i);
					}
				}
				else if (mathOperator == "<=") {
					if (AllTableData[i][columnIndex] > ifValue) {
						toDelete.push_back(i);
					}
				}
				else if (mathOperator == ">=") {
					if (AllTableData[i][columnIndex] < ifValue) {
						toDelete.push_back(i);
					}
				}
			}
			i++;
		}
		int deletedCount = 0;
		for (int idx : toDelete) {
			result.erase(result.begin() + (idx - deletedCount));
			deletedCount++;
		}
	}
	return result;
}