#pragma once

class Updatable {
  public:
    virtual void update(float delta_time) {}
    virtual ~Updatable() = default;
};
