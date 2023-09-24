#include "LevelManager.hpp"

namespace engine {

ErrorOr<std::shared_ptr<Level>> LevelManager::start_next(Window& window,
                                                         AssetManager<sf::Texture>& textures) {
    if (cur_num_ + 1 > level_num_) {
        return Error::New("No such level");
    }
    return get(cur_num_ + 1, window, textures);
}

ErrorOr<std::shared_ptr<Level>> LevelManager::get(size_t num, Window& window,
                                                  AssetManager<sf::Texture>& textures) {
    auto level = loader_.load_level(window, textures, num);
    if (!level) {
        return tl::unexpected(level.error());
    }

    cur_num_ = num;
    return std::make_shared<Level>(std::move(level.value()));
}
}  // namespace engine
