#include "LevelManager.hpp"

namespace engine {

ErrorOr<std::shared_ptr<Level>> LevelManager::start_next(Window& window) {
    if (cur_num_ + 1 > level_num_) {
        return Error::New("No such level");
    }
    return get(cur_num_ + 1, window);
}

ErrorOr<std::shared_ptr<Level>> LevelManager::get(size_t num, Window& window) {
    auto level = loader_.load_level(window, num);
    if (!level) {
        return tl::unexpected(level.error());
    }

    cur_num_ = num;
    return std::make_shared<Level>(std::move(level.value()));
}

ErrorOr<std::shared_ptr<Level>> LevelManager::reload(Window& window) {
    return get(cur_num_, window);
}
}  // namespace engine
