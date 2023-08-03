#pragma once

#include <deque>
#include <tl/expected.hpp>

#include "Errors.hpp"
#include "Handler.hpp"
#include "Json.hpp"
#include "ObjectOptions.hpp"

class ObjectOptionsFactory {
  public:
    using res_type = std::deque<ObjectOptions>;

    ErrorOr<ObjectOptions> generate(const nl::json& value) const;

    ErrorOr<res_type> generate(const std::vector<nl::json>& json) const;

  private:
    static HandlerChain<ObjectOptions> handler_chain_;
};
