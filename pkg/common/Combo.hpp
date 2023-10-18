#pragma once

#include <algorithm>
#include <string>

namespace combo {
inline std::string find_item(const std::string& input, int idx) {
    int cur = 0;
    int i = 0;

    for (auto chr : input) {
        if (cur == idx) {
            return input.substr(i, input.substr(i).find('\0'));
        }

        if (chr == '\0') {
            ++cur;
        }

        ++i;
    }
    return "";
}

inline int get_item_id(const std::string& types, const std::string& to_find) {
    return std::ranges::count(types.substr(0, types.find(to_find)), '\0');
}
}  // namespace combo
