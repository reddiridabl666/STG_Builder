#include "LevelManager.hpp"

namespace engine {

ErrorOr<std::shared_ptr<Level>> LevelManager::start_next(Window& window) {
    if (cur_num_ + 1 > level_num_) {
        return Error::New("No such level");
    }
    return get(cur_num_ + 1, window);
}

ErrorOr<std::shared_ptr<Level>> LevelManager::get(size_t num, Window& window) {
    try {
        auto level = loader_.load_level(window, num);
        cur_num_ = num;
        return std::make_shared<Level>(std::move(level));
    } catch (Error& e) {
        return tl::unexpected(e);
    }
}

ErrorOr<std::shared_ptr<Level>> LevelManager::reload(Window& window) {
    return get(cur_num_, window);
}

void LevelManager::reset() {
    cur_num_ = 0;
}
}  // namespace engine
