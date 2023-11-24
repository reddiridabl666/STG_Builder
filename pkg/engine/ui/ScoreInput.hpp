#pragma once

#include "Button.hpp"
#include "Form.hpp"
#include "TextInput.hpp"
#include "VerticalLayout.hpp"

namespace engine {
class ScoreInput : public Form<VerticalLayout> {
  public:
    ScoreInput(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg, std::unique_ptr<TextInput>&& input,
               std::unique_ptr<Button>&& submit, float offset);
};
}  // namespace engine
