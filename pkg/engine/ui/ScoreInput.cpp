#include "ScoreInput.hpp"

#include "GameBus.hpp"

engine::ScoreInput::ScoreInput(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& msg,
                               std::unique_ptr<TextInput>&& input, std::unique_ptr<Button>&& submit, float offset)
    : Form<VerticalLayout>(std::move(bg), offset) {
    input->set_cb([](const std::string& name) {
        GameBus::get().emit(GameEvent::NameEntered, name);
    });

    submit->set_cb([input = input.get()] {
        GameBus::get().emit(GameEvent::NameEntered, input->get_text());
    });

    layout_.add_items(std::move(msg), std::move(input), std::move(submit));
    layout_.init(1);
}
