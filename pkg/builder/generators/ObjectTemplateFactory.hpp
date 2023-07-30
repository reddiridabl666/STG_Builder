#pragma once

#include <expected>
#include <string>
#include <string_view>

#include "Errors.hpp"
#include "Json.hpp"
#include "ObjectTemplate.hpp"

class ObjectTemplateFactory {
  public:
    tl::expected<ObjectTemplate, ErrorPtr> generate(const std::string& name, nlohmann::json json) const;
};
