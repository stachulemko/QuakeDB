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
void showVariantVariable(allVars vec) {
    if (std::holds_alternative<int32_t>(vec)) {
        std::cout << std::get<int32_t>(vec);
    }
    else if (std::holds_alternative<int64_t>(vec)) {
        std::cout << std::get<int64_t>(vec);
    }
    else if (std::holds_alternative<std::string>(vec)) {
        std::cout << std::get<std::string>(vec);
    }
    else {
        std::cout << "NULL";
    }
}