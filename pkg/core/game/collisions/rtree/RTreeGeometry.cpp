#include "RTreeGeometry.hpp"

RTreeGeometry::RTreeGeometry(const std::string& id, std::unique_ptr<Framed>&& frame, RTree<>& rtree)
    : Identifiable<std::string>(id), frame_(std::move(frame)), rtree_(rtree) {
    old_frame_ = frame_->get_frame();
    rtree_.insert(id, old_frame_);
}

void RTreeGeometry::update() {
    auto cur_frame = frame_->get_frame();
    Event<std::string, FloatBox> event{id(), cur_frame};

    rtree_.remove(id(), old_frame_);
    rtree_.insert(id(), cur_frame);

    old_frame_ = cur_frame;
}
