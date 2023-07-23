#include "EntityGenerator.hpp"

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

template <typename T>
std::pair<T, bool> try_get(json::value value, std::string_view key) {
    try {
        T res = json::value_to<T>(value.at(key));
        return std::make_pair(res, true);
    } catch (json::error& _) {
        return std::make_pair(T{}, false);
    }
}

[[maybe_unused]] static bool has_value(json::value value, std::string_view key) {
    try {
        value.at(key);
        return true;
    } catch (json::error& _) {
        return false;
    }
}

static std::string capitalize(const std::string& str) {
    auto res = str;
    res[0] = toupper(res[0]);
    return res;
}

static const std::vector<std::string> value_classes = {"health", "speed", "score"};

static std::unordered_map<std::string, std::string> get_value_classes(json::value value) {
    std::unordered_map<std::string, std::string> res;
    res.reserve(3);

    for (const auto& name : value_classes) {
        auto [val, ok] = try_get<int>(value, name);
        if (ok) {
            auto capitalized = capitalize(name);
            res[capitalized] = capitalized + "(" + std::to_string(val) + ")";
        }
    }

    return res;
}

std::pair<std::string, bool> EntityGenerator::generate(json::value value) {
    std::stringstream out;

    auto [class_name, ok] = try_get<std::string>(value, "name");
    if (!ok) {
        return std::make_pair("", false);
    }

    class_name = kPrefix_ + class_name;
    auto value_classes = get_value_classes(value);

    out << "class " << class_name << " : public GameObject";
    if (!value_classes.empty()) {
        out << ", ";
    }

    for (auto it = value_classes.begin(); it != value_classes.end(); ++it) {
        out << "public " << it->first;
        if (std::next(it) != value_classes.end()) {
            out << ", ";
        }
    }

    out << " {\n"
        << "public:\n"
        << class_name << "(std::shared_ptr<sf::Texture>&& tx) : GameObject(tx)";

    for (auto it = value_classes.begin(); it != value_classes.end(); ++it) {
        out << it->second;
        if (std::next(it) != value_classes.end()) {
            out << ", ";
        }
    }

    out << "{}";

    return std::make_pair(out.str(), true);
}
