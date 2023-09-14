#include "LevelTab.hpp"

#include "Messages.hpp"

namespace ui {
namespace {
struct FieldOptions {
    float speed;
    std::string image;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FieldOptions, speed, image)

struct LevelTabContents : public Element {
  public:
    std::string name;
    FieldOptions field;

    void draw(const Window&) override {
        ImGui::InputText(message(Message::Name), &name);

        ImGui::InputText("Background", &field.image);
        ImGui::InputFloat("Speed", &field.speed);
    }
};
}  // namespace

Menu::Tab ui::LevelTab(nl::json& json) {
    auto tab = std::make_unique<LevelTabContents>(json);

    tab->name = json.at("name").template get<std::string>();
    tab->field = json.at("bg").template get<FieldOptions>();

    return Menu::Tab(std::move(tab), message_func(Message::LevelOpts));
}
}  // namespace ui
