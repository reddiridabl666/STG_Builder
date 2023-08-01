#pragma once

#include <tl/expected.hpp>

#include "Errors.hpp"
#include "Handler.hpp"
#include "Json.hpp"
#include "ObjectTemplate.hpp"

class ObjectTemplateFactory {
  public:
    tl::expected<ObjectTemplate, ErrorPtr> generate(const std::string& name, const nl::json& json) const;

  private:
    static HandlerChain<ObjectTemplate> handler_chain_;
};
