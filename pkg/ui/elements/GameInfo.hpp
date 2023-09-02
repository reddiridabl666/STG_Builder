#pragma once

#include <cstddef>
#include <string>

#include "Button.hpp"
#include "Common.hpp"
#include "Json.hpp"
#include "UiElement.hpp"

namespace ui {
struct GameInfo : public Element {
  public:
    static constexpr const char* kDefaultDesc = "Description does here";

    GameInfo() = default;
    GameInfo(const std::string& name, const std::string& desc = kDefaultDesc)
        : name(name), description(desc) {}

    template <typename Cb>
    void set_cb(Cb&& cb) {
        cb_ = std::forward<Cb>(cb);
    }

    std::string name = "";
    std::string description = kDefaultDesc;
    size_t levels = 0;
    time_t last_updated = time(nullptr);

    void draw(const Window&) const override;

  private:
    std::function<void()> cb_;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(GameInfo, name, description, levels, last_updated);
}  // namespace ui
