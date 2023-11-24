#pragma once

#include "Button.hpp"
#include "Form.hpp"
#include "VerticalLayout.hpp"

namespace engine {
class GameOver : public Form<VerticalLayout> {
  public:
    GameOver(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg, std::unique_ptr<Button>&& retry,
             std::unique_ptr<Button>&& quit, float offset, float margin);
};
}  // namespace engine
