#include "typeManager.h"

int32_t getTypeId(const allVars& value) {
    if (std::holds_alternative<int32_t>(value)) {
        return int32_tId;
    }
    else if (std::holds_alternative<int64_t>(value)) {
        return int64_tId;
    }
    else if (std::holds_alternative<std::string>(value)) {
        return stringId;
    }
    return -1;
}
Tlv* convertVarToTlv(const allVars& var) {
    if (std::holds_alternative<int32_t>(var)) {
        return new Tlv(std::get<int32_t>(var));
    }
    else if (std::holds_alternative<int64_t>(var)) {
        return new Tlv(std::get<int64_t>(var));
    }
    else if (std::holds_alternative<std::string>(var)) {
        return new Tlv(std::get<std::string>(var));
    }
    return nullptr;
}