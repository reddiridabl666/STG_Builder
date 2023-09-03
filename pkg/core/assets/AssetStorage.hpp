#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <tl/expected.hpp>
#include <unordered_map>

#include "Loadable.hpp"
#include "Utils.hpp"

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
            return Error::NoKey(filename);
        }
        return assets_.at(filename).lock();
    }

    void clear_unused() {
        std::erase_if(assets_, [](const auto& el) {
            return el.second.expired();
        });
    }

    friend std::ostream& operator<<(std::ostream& out, const AssetStorage& storage) {
        return out << storage.assets_;
    }

  private:
    std::unordered_map<std::string, std::weak_ptr<T>> assets_;
};
