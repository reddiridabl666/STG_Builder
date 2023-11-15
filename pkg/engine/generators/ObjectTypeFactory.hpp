#pragma once

#include "Handler.hpp"
#include "Json.hpp"
#include "ObjectType.hpp"

class ObjectTypeFactory {
  public:
    using res_type = std::unordered_map<std::string, ObjectType>;

    static ObjectType generate(const std::string& name, const nl::json& json);

    static res_type generate(const nl::json& json);

  private:
    static HandlerChain<ObjectType> handler_chain_;
};
