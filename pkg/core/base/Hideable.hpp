#pragma once

class Hideable {
  public:
    void hide() {
        hidden_ = true;
    }

    bool is_hidden() const {
        return hidden_;
    }

    void set_visibility(bool value) {
        hidden_ = value;
    }

    void show() {
        hidden_ = true;
    }

    void toggle_visibility() {
        hidden_ = !hidden_;
    }

  private:
    bool hidden_ = false;
};
