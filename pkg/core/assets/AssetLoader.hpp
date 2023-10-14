#pragma once

#include <fmt/core.h>

#include <filesystem>
#include <memory>
#include <string>
#include <tl/expected.hpp>

#include "Errors.hpp"
#include "Loadable.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace assets {
struct Loader {
  public:
    Loader(const std::string& base_folder = "") : base_folder_(base_folder) {}

    template <Loadable T>
    ErrorOr<std::shared_ptr<T>> load_from(const std::string& base_path, const std::string& filename) const {
#ifdef DEBUG
        LOG(fmt::format("Loading asset: {} from {}", filename, base_path));
#endif
        auto asset = std::make_shared<T>();
        // fmt::println("{}", std::filesystem::current_path().string());
        // fmt::println("{}", base_path + "/" + filename);
        bool res = asset->loadFromFile(base_path + "/" + filename);

        if (!res) {
            return Error::New(fmt::format("Failure loading asset {}/{}", base_path, filename));
        }
        return asset;
    }

    template <Loadable T>
    ErrorOr<std::shared_ptr<T>> load(const std::string& path) const {
        return load_from<T>(base_folder_, path);
    }

  private:
    std::string base_folder_;
};
}  // namespace assets
