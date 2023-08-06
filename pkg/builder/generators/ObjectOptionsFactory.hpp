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

    ErrorOr<ObjectOptions> generate(const GameField& field, const nl::json& value) const;

    ErrorOr<res_type> generate(const GameField& field, const std::vector<nl::json>& json) const;

  private:
    HandlerChain<ObjectOptions> handler_chain_;
};
