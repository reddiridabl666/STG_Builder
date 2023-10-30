#pragma once

#include "Displayable.hpp"

class GameUi : virtual public Displayable {
  public:
    virtual void update(float) = 0;
};
