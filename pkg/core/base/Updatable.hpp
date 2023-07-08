#pragma once

class Updatable {
  public:
    virtual void update() {}
    virtual ~Updatable() = default;
};
