#pragma once

#include "Button.hpp"
#include "Form.hpp"
#include "VerticalLayout.hpp"

namespace engine {
class MainMenu : public Form<VerticalLayout> {
  public:
    MainMenu(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg, std::unique_ptr<Button>&& start,
             std::unique_ptr<Button>&& settings, std::unique_ptr<Button>&& leaderboards, std::unique_ptr<Button>&& quit,
             float offset);
};
}  // namespace engine
