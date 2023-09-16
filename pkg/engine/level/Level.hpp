#pragma once

#include "GameField.hpp"
#include "ObjectOptionsFactory.hpp"

namespace engine {

class Level {
  public:
    Level(const std::string& name, GameField&& field, ObjectOptionsFactory::res_type&& objects);

    bool has_ended();

    const std::string& name() const {
        return name_;
    }

    const GameField& field() const {
        return field_;
    }

    GameField& field() {
        return field_;
    }

    auto& objects() {
        return objects_;
    }

  private:
    void prepare_objects();

    std::string name_;
    GameField field_;
    ObjectOptionsFactory::res_type objects_;
};
}  // namespace engine
