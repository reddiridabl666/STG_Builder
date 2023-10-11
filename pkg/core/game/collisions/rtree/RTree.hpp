#pragma once

#include <boost/geometry/index/rtree.hpp>
#include <utility>

#include "Geometry.hpp"

namespace rtree = boost::geometry::index;

template <typename index_type = std::string, typename box_type = FloatBox>
class RTree {
  public:
    using value_type = std::pair<box_type, index_type>;

    virtual void insert(const index_type& index, const box_type& box) {
        rtree_.insert(std::pair(box, index));
    }

    virtual void remove(const index_type& index, const box_type& box) {
        rtree_.remove(std::pair(box, index));
    }

    auto intersects(const box_type& box) const {
        return rtree_.qbegin(rtree::intersects(box));
    }

    template <typename Geometry>
    auto contains(const Geometry& geom) const {
        return rtree_.qbegin(rtree::contains(geom));
    }

    auto qend() const {
        return rtree_.qend();
    }

    auto end() const {
        return rtree_.end();
    }

    auto begin() const {
        return rtree_.begin();
    }

    virtual void clear() {
        rtree_.clear();
    }

  private:
    using algorithm = rtree::quadratic<16>;

    rtree::rtree<value_type, algorithm> rtree_;
};
