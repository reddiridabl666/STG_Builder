#pragma once

class Updatable {
  public:
    virtual void update(float) {}
    virtual ~Updatable() = default;
};
