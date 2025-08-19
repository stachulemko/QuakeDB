#include "join.h"

std::vector<std::vector<allVars>> joinFunction(
    std::vector<std::vector<allVars>> lastStateFirstTableData,
    std::vector<std::vector<allVars>> allFirstTableData,
    std::vector<std::vector<allVars>> allSecondTableData,
    std::string secondTableColumnName,
    std::string firstTableColumnName)
{
    std::vector<std::vector<allVars>> result;
    result.push_back(lastStateFirstTableData[0]);
    int firstColumnIndex = -1;
    int secondColumnIndex = -1;

    for (int i = 0; i < allFirstTableData[0].size(); i++) {
        bool isString = std::holds_alternative<std::string>(allFirstTableData[0][i]);

        if (isString) {
            std::string colName = std::get<std::string>(allFirstTableData[0][i]);
            if (colName == firstTableColumnName) {
                firstColumnIndex = i;
                break;
            }
        }
        else {
            // Usuniêto niepotrzebne cout
        }
    }

    for (int i = 0; i < allSecondTableData[0].size(); i++) {
        if (std::holds_alternative<std::string>(allSecondTableData[0][i]) &&
            std::get<std::string>(allSecondTableData[0][i]) == secondTableColumnName) {
            secondColumnIndex = i;
            break;
        }
    }

    std::vector<int> indexes;
    if (allFirstTableData.size() > allSecondTableData.size()) {
        std::vector<allVars> values;  // Poprawiono z Allvars na allVars
        for (int i = 0; i < allSecondTableData.size(); i++) {
            values.push_back(allSecondTableData[i][secondColumnIndex]);
        }
        for (int i = 1; i < allFirstTableData.size(); i++) {
            bool wasFound = false;
            for (int j = 0; j < values.size(); j++) {
                if (values[j] == allFirstTableData[i][firstColumnIndex]) {
                    indexes.push_back(i);
                    wasFound = true;
                }
            }
            if (wasFound == false) {  // Poprawiono z = na ==
                indexes.push_back(-1);
            }
        }
    }
    else {
        std::vector<allVars> values;  // Poprawiono z Allvars na allVars
        for (int i = 0; i < allFirstTableData.size(); i++) {
            values.push_back(allFirstTableData[i][firstColumnIndex]);
        }
        for (int i = 1; i < allSecondTableData.size(); i++) {
            bool wasFound = false;
            for (int j = 0; j < values.size(); j++) {
                if (values[j] == allSecondTableData[i][secondColumnIndex]) {
                    indexes.push_back(i);
                    wasFound = true;
                }
            }
            if (wasFound == false) {  // Poprawiono z = na ==
                indexes.push_back(-1);
            }
        }
    }

    for (int i = 0; i < indexes.size(); i++) {
        if (indexes[i] != -1) {
            result.push_back(lastStateFirstTableData[indexes[i]]);
        }
        else {
            std::vector<allVars> tmp;
            for (int j = 0; j < allFirstTableData[0].size(); j++) {
                tmp.push_back("NULL");
            }
            result.push_back(tmp);
        }
    }

    return result;
}