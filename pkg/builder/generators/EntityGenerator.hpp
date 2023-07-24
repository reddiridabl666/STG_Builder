#pragma once

#include <boost/json/src.hpp>
#include <expected>
#include <string>
#include <string_view>

#include "Errors.hpp"

namespace json = boost::json;

class EntityGenerator {
  private:
    static inline const std::string kPrefix_ = "gen_";

  public:
    std::expected<std::string, Error> generate(json::value value);
};
