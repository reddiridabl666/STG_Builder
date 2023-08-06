#pragma once

#include "GameObject.hpp"

class EnemyObject : public GameObject {
  public:
    EnemyObject(std::unique_ptr<Displayable>&& image, int speed = 50, const Properties::Data& props = {});

    ~EnemyObject();
};
