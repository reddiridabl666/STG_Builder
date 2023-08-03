#include "LevelLoader.hpp"

#include <fmt/core.h>

#include <fstream>
#include <tl/expected.hpp>

#include "Json.hpp"

ErrorOr<Level> LevelLoader::load_level(size_t number) const {
    auto j = json::read(fmt::format("{}_{}", prefix, number));
    if (!j) {
        return tl::unexpected(j.error());
    }

    ObjectOptionsFactory factory;
    auto opts = factory.generate(j->at("entities").get<std::vector<nl::json>>());
    if (!opts) {
        return tl::unexpected(opts.error());
    }

    try {
        return Level{
            .name = (*j)["name"].get<std::string>(),
            .objects = std::move(opts.value()),
        };
    } catch (std::exception& e) {
        return unexpected_error<InternalError>(e.what());
    }
}
