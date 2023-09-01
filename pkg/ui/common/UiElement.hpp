#pragma once

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <SFML/System/Vector2.hpp>

#include "Window.hpp"

namespace ui {
class Element {
  public:
    Element(const ImVec2& pos = {}, const ImVec2 size = {}) {
        set_pos(pos);
        set_size(size);
    }

    virtual void draw(const Window&) const = 0;
    virtual ~Element() = default;

    const ImVec2& get_pos() const {
        return pos_;
    }

    virtual void set_pos(const ImVec2& pos) {
        pos_ = pos;
    }

    const ImVec2& get_size() const {
        return size_;
    }

    void set_size(const ImVec2& size) {
        size_ = size;
    }

  private:
    ImVec2 pos_;
    ImVec2 size_;
};
}  // namespace ui
