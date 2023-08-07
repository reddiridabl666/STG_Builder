#include "LevelManager.hpp"

ErrorOr<std::shared_ptr<Level>> LevelManager::start_next(Window& window,
                                                         AssetManager<sf::Texture>& textures) {
    if (cur_num_ == level_num_) {
        return unexpected_error<InternalError>("No levels left");
    }

    auto level = loader_.load_level(window, textures, cur_num_ + 1);
    if (!level) {
        return tl::unexpected(level.error());
    }

    ++cur_num_;
    return std::make_shared<Level>(std::move(level.value()));
}
