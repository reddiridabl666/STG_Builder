#pragma once

// #include <nlohmann/json.hpp>
#include <boost/json/src.hpp>
#include <string>
#include <string_view>

namespace json = boost::json;

class EntityGenerator {
  private:
    static inline const std::string kPrefix_ = "gen_";

  public:
    std::pair<std::string, bool> generate(json::value value);
};
