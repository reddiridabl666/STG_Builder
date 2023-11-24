#include "ScoreInput.hpp"

engine::ScoreInput::ScoreInput(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
                               std::unique_ptr<TextInput>&& input, std::unique_ptr<Button>&& submit, float offset)
    : Form<VerticalLayout>(
          VerticalLayout(std::move(bg), offset, 1, std::move(msg), std::move(input), std::move(submit))) {}
