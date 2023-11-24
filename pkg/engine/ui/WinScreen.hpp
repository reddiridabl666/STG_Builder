#pragma once

#include "Button.hpp"
#include "Form.hpp"
#include "VerticalLayout.hpp"

namespace engine {
class WinScreen : public Form<VerticalLayout> {
  public:
    WinScreen(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg, std::unique_ptr<Button>&& save_score,
              std::unique_ptr<Button>&& quit, float offset_x, float offset_y);
};
}  // namespace engine
