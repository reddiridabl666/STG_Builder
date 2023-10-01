#pragma once

#include <algorithm>
#include <string>
#include <tl/expected.hpp>
#include <unordered_map>

#include "Errors.hpp"
#include "Value.hpp"

struct Properties {
  public:
    using Data = std::unordered_map<std::string, Value<>>;

    Properties(Data data = Data{}) : values_(std::move(data)) {}

    bool has(const std::string& key) const {
        return values_.contains(key);
    }

    Value<>& at(const std::string& key) {
        return values_.at(key);
    }

    const Value<>& at(const std::string& key) const {
        return const_cast<Properties*>(this)->at(key);
    }

    void set(const std::string& key, int value) {
        values_[key] = value;
    }

    void append_to(Properties& other) const {
        std::for_each(values_.begin(), values_.end(), [&other](auto&& el) {
            other.values_[el.first] = el.second;
        });
    }

    void append(const Properties& other) {
        other.append_to(*this);
    }

  private:
    std::unordered_map<std::string, Value<>> values_;
};
