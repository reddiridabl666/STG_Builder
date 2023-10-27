#pragma once

#include "Displayable.hpp"

class GameUi : public Displayable {
    virtual void update(float) = 0;
};
