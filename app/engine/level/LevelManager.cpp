#include "LevelManager.hpp"

ErrorOr<std::shared_ptr<Level>> LevelManager::current() {
    if (cur_num_ == 0) {
        return unexpected_error<InternalError>("No levels loaded");
    }
    return cur_level_;
}

ErrorOr<std::shared_ptr<Level>> LevelManager::start_next() {
    if (cur_num_ == level_num_) {
        return unexpected_error<InternalError>("No levels left");
    }

    auto level = loader_.load_level(cur_num_ + 1);
    if (!level) {
        return tl::unexpected(level.error());
    }

    ++cur_num_;
    cur_level_ = std::make_shared<Level>(std::move(level.value()));
    return cur_level_;
}
