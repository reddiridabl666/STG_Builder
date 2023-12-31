#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>

#include "Errors.hpp"
#include "Value.hpp"

struct Properties {
  public:
    using Data = std::unordered_map<std::string, Value>;

    Properties(Data data = Data{}) : values_(std::move(data)) {}

    bool has(const std::string& key) const {
        return values_.contains(key);
    }

    Value& at(const std::string& key) {
        auto it = values_.find(key);
        if (it == values_.end()) {
            return dummy_;
        }
        return it->second;
    }

    Value get(const std::string& key) const {
        auto it = values_.find(key);
        if (it == values_.end()) {
            return 0;
        }
        return it->second;
    }

    const Value& at(const std::string& key) const {
        return const_cast<Properties*>(this)->at(key);
    }

    void set(const std::string& key, Value value) {
        values_[key] = value;
    }

    void reset() {
        for (auto& [_, val] : values_) {
            val.reset();
        }
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
    std::unordered_map<std::string, Value> values_;
    Value dummy_;
};
