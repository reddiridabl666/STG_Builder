#pragma once

#include "Displayable.hpp"

class StatUi : virtual public Displayable {
  public:
    virtual void update(float) = 0;
};
