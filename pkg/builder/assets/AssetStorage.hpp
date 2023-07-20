#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>

#include "Loadable.hpp"

template <Loadable T>
class AssetStorage {
  public:
    void add(const std::string& filename, const std::shared_ptr<T>& asset) {
        assets_[filename] = asset;
    }

    bool exists(const std::string& filename) const {
        return assets_.contains(filename) && !assets_[filename].expired();
    }

    std::shared_ptr<T> get(const std::string& filename) const {
        return assets_[filename].lock();  // TODO: Добавить ошибку
    }

    void clear_unused() {
        std::remove_if(assets_.begin(), assets_.end(),
                       [&res](const auto& el) { return el.second.expired(); });
    }

  private:
    std::unordered_map<std::string, std::weak_ptr<T>> assets_;
};
