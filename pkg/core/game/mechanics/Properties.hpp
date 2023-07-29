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

    Properties(Data data = Data{})
        : values_(std::move(data)) {}

    bool has(const std::string& key) const {
        return values_.contains(key);
    }

    tl::expected<std::reference_wrapper<Value<>>, NoKeyError> operator[](const std::string& key) noexcept {
        if (!has(key)) {
            return tl::unexpected(NoKeyError(key));
        }
        return std::ref(values_[key]);
    }

    Value<>& value(const std::string& key) {
        return values_.at(key);
    }

    void add(const std::string& key, int value) {
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
