#pragma once

#include <memory>

#include "Bar.hpp"
#include "Counter.hpp"
#include "Json.hpp"

class StatDisplayFactory {
    template <typename T = int>
    std::unique_ptr<StatDisplay> create(Value<T>&, const nl::json&);
};

template <typename T>
std::unique_ptr<StatDisplay> StatDisplayFactory::create(Value<T>& val, const nl::json& json) {
    auto type = json::get<std::string>(json, "type");
    if (type == "bar") {
        return std::make_unique<Bar<T>>(val);
    }

    if (type == "counter") {
        return std::make_unique<Counter<T>>(val);
    }
}
