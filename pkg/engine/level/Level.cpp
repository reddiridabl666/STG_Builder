#include "Level.hpp"

#include "GameInfo.hpp"

namespace engine {
void Level::prepare_objects() {
    std::sort(objects_.begin(), objects_.end());
}

Level::Level(const std::string& name, GameField&& field, ObjectOptionsFactory::res_type&& objects)
    : name_(name), field_(std::move(field)), objects_(std::move(objects)) {
    prepare_objects();
}

bool Level::has_ended() {
    // TODO: Custom checks?
    return field_.has_ended() && GameState::get().enemy_count() == 0;
}
}  // namespace engine
