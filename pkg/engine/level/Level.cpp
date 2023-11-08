#include "Level.hpp"

#include "GameInfo.hpp"

namespace engine {
void Level::prepare_for_load() {
    std::sort(not_loaded_objects_.begin(), not_loaded_objects_.end());
}

Level::Level(const std::string& name, GameField&& field, ObjectOptionsFactory::res_type&& objects)
    : name_(name), field_(std::move(field)), not_loaded_objects_(std::move(objects)) {
    prepare_for_load();
}

bool Level::has_ended() {
    // TODO: Custom checks?
    return field_.has_ended() && GameState::get().enemy_count() == 0;
}
}  // namespace engine
