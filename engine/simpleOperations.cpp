#include "simpleOperations.h"

bool isStringInVector(std::string str, std::vector<std::string>vec) {
	for (const auto& item : vec) {
		if (item == str) {
			return true;
		}
	}
	return false;
}