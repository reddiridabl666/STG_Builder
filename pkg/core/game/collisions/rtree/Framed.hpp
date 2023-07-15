#pragma once

#include "Geometry.hpp"

class Framed {
  public:
    virtual FloatBox get_frame() const = 0;
    virtual ~Framed() = default;
};
