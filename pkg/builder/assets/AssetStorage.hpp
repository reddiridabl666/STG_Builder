#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <tl/expected.hpp>
#include <unordered_map>

#include "Loadable.hpp"

template <Loadable T>
class AssetStorage {
  public:
    void add(const std::string& filename, const std::shared_ptr<T>& asset) {
        assets_[filename] = asset;
    }

    bool exists(const std::string& filename) const noexcept {
        return assets_.contains(filename) && !assets_.at(filename).expired();
    }

    ErrorOr<std::shared_ptr<T>> get(const std::string& filename) const noexcept {
        if (!exists(filename)) {
            return unexpected_error<NoKeyError>(filename);
        }
        return assets_.at(filename).lock();
    }

    void clear_unused() {
        std::remove_if(assets_.begin(), assets_.end(), [](const auto& el) {
            return el.second.expired();
        });
    }

  private:
    std::unordered_map<std::string, std::weak_ptr<T>> assets_;
};
