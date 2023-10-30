#pragma once

#include "Displayable.hpp"

class GameUi : virtual public Displayable {
    virtual void update(float) = 0;
};
