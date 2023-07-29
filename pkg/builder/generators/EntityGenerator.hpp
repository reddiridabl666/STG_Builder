#pragma once

#include <expected>
#include <string>
#include <string_view>

#include "Errors.hpp"
#include "Json.hpp"
#include "ObjectTemplate.hpp"

class EntityGenerator {
  public:
    tl::expected<ObjectTemplate, Error> generate(nlohmann::json value);
};
