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

    auto& object_props() {
        return not_loaded_objects_;
    }

    void prepare_for_load();

  private:
    std::string name_;
    GameField field_;
    ObjectOptionsFactory::res_type not_loaded_objects_;
};
}  // namespace engine
