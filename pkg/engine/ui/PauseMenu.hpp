#pragma once

#include "Button.hpp"
#include "Form.hpp"
#include "VerticalLayout.hpp"

namespace engine {
class PauseMenu : public Form<VerticalLayout> {
  public:
    PauseMenu(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg, std::unique_ptr<Button>&& continue_btn,
              std::unique_ptr<Button>&& quit, float offset);
};
}  // namespace engine
