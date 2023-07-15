#pragma once

#include <boost/geometry/index/rtree.hpp>
#include <utility>

#include "Geometry.hpp"
#include "Observable.hpp"

namespace rtree = boost::geometry::index;

template <typename index_type = std::string, typename box_type = FloatBox>
class RTree : public Observable<index_type, box_type> {
  public:
    using value_type = std::pair<box_type, index_type>;

    void insert(index_type index, const box_type& box) {
        rtree_.insert(std::make_pair(box, index));
    }

    void remove(index_type index, const box_type& box) {
        rtree_.remove(std::make_pair(box, index));
    }

    void remove(const value_type& value) {
        rtree_.remove(value);
    }

    void insert(const value_type& value) {
        rtree_.insert(value);
    }

    const auto& intersects(const box_type& box) const {
        return rtree_.qbegin(rtree::intersects(box));
    }

  private:
    using algorithm = rtree::quadratic<16>;

    rtree::rtree<value_type, algorithm> rtree_;
};
