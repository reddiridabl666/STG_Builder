#pragma once

#include <tl/expected.hpp>

#include "Errors.hpp"
#include "Handler.hpp"
#include "Json.hpp"
#include "ObjectType.hpp"

class ObjectTypeFactory {
  public:
    using res_type = std::unordered_map<std::string, ObjectType>;

    static ErrorOr<ObjectType> generate(const std::string& name, const nl::json& json);

    static ErrorOr<res_type> generate(const nl::json& json);

  private:
    static HandlerChain<ObjectType> handler_chain_;
};