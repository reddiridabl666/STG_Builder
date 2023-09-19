#include "LevelTab.hpp"

#include "Messages.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace ui {
namespace {
struct FieldOptions {
    float speed = 0;
    std::string image = "";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(FieldOptions, speed, image);

struct LevelTabContents : public Element {
  public:
    LevelTabContents(nl::json& json) : data(json) {}

    std::string name;
    FieldOptions field;

    void draw(const Window&) override {
        ImGui::InputText(message(Message::Name), &name);

        ImGui::SeparatorText(message(Message::GameField));
        ImGui::InputText(message(Message::Image), &field.image);
        ImGui::InputFloat(message(Message::Speed), &field.speed);
    }

    ~LevelTabContents() {
        try {
            data["name"] = name;
            data["bg"] = field;
            data["last_updated"] = time(nullptr);
        } catch (std::exception& e) {
#ifdef DEBUG
            LOG(e.what());
#endif
        }
    }

  private:
    nl::json& data;
};
}  // namespace

Menu::Tab LevelTab(nl::json& json) {
    auto tab = std::make_unique<LevelTabContents>(json);

    try {
        tab->name = json.at("name").template get<std::string>();
        tab->field = json.at("bg").template get<FieldOptions>();
    } catch (std::exception& e) {
        fmt::println("{}", e.what());
    }

    return Menu::Tab(std::move(tab), message_func(Message::LevelOpts));
}
}  // namespace ui
