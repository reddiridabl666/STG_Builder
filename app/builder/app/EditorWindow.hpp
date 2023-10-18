#pragma once

#include "Window.hpp"
#include "WindowInfo.hpp"

namespace builder {
class EditorWindow : public Window {
  public:
    static EditorWindow& get() {
        static EditorWindow window(params.name, params.size.x, params.size.y);
        return window;
    }

    static void set_params(const WindowInfo& info) {
        params = info;
    }

    static EditorWindow& init(const std::string& name, uint width, uint height) {
        set_params(WindowInfo{name, sf::Vector2u{width, height}});
        return get();
    }

    EditorWindow(const EditorWindow&) = delete;
    EditorWindow& operator=(const EditorWindow&) = delete;

    EditorWindow(EditorWindow&&) = delete;
    EditorWindow& operator=(EditorWindow&&) = delete;

  private:
    using Window::Window;
    static inline WindowInfo params = {};
};
}  // namespace builder
