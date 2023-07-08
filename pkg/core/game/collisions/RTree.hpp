#pragma once

#include <boost/geometry/index/rtree.hpp>
#include <utility>

#include "Geometry.hpp"

namespace rtree = boost::geometry::index;

template <typename index_type, typename box_type = FloatBox>
class RTree {
  public:
    using value_type = std::pair<box_type, index_type>;

    void insert(const box_type& box, index_type index) {
        rtree_.insert(std::make_pair(box, index));
    }

    void remove(const box_type& box, index_type index) {
        rtree_.remove(std::make_pair(box, index));
    }

    const auto& intersects(const box_type& box) const {
        return rtree_.qbegin(rtree::intersects(box));
    }

  private:
    using algorithm = rtree::quadratic<16>;

    rtree::rtree<value_type, algorithm> rtree_;
};
