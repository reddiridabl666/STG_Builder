#pragma once

#include <tl/expected.hpp>

#include "Errors.hpp"
#include "Handler.hpp"
#include "Json.hpp"
#include "ObjectOptions.hpp"

class ObjectOptionsFactory {
  public:
    tl::expected<ObjectOptions, ErrorPtr> generate(const nl::json& value) const;

  private:
    static HandlerChain<ObjectOptions> handler_chain_;
};
