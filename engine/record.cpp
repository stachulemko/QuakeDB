#include "record.h"

Record::Record(std::vector<allVars> recordData) {
	this->recordData = recordData;
}
bool Record::isDataTypeCorrect(std::vector<allVars> recordData,std::vector<Column*> vec) {
	if (vec.size() != recordData.size()) {
		std::cerr << "Error(insert): Number of columns does not match number of data types in record." << std::endl;
		return false;
	}
	else {
		for (int i = 0; i < recordData.size(); i++) {
			//if (getTypeId(recordData[i]) != vec[i]->getColumnType()) {
			//	return false;
			//}
		}
	}
	return true;

}