#pragma once

template <typename T>
class Identifiable {
  public:
    explicit Identifiable(const T& id) : id(id_) {}

    const T& id() {
        return id_;
    }

  private:
    T id_;
};
