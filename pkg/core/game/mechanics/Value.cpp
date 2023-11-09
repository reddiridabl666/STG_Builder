#include "Value.hpp"

void from_json(const nl::json& json, Value& value) {
    if (json.is_number()) {
        value.default_ = json.get<float>();
        value.val_ = value.default_;
        return;
    }

    value.default_ = json.value("default", 0);
    value.val_ = json.value("value", 0);
}

void to_json(nl::json& json, const Value& value) {
    if (std::abs(value.default_value()) < 1e-7) {
        json = value.get();
        return;
    }

    json["value"] = value.get();
    json["default"] = value.default_value();
}
