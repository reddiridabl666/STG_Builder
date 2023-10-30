#include "MainTab.hpp"

#include "Bus.hpp"
#include "ImguiUtils.hpp"
#include "Json.hpp"
#include "Messages.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace ui {

namespace {
struct SideMenuOptions {
    sf::FloatRect size;
    std::vector<std::pair<std::string, float>> stats;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SideMenuOptions, size, stats)

struct MainTabContents : public Element {
  public:
    MainTabContents(nl::json& data) : data(data) {}

    void draw(const Window&) override {
        ImGui::InputText(message(Message::Name), &name);
        ImGui::InputTextMultiline(message(Message::Desc), &description);
        ImGui::NewLine();

        ImGui::InputText(message(Message::Background), &bg);
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            Bus<std::string>::get().emit(Event::GameBgChanged, bg);
        }

        ImGui::SeparatorText(message(Message::GameField));
        ImGui::Text(message(Message::GameFieldHint));

        ImGui::SizeInput(message(Message::Size), &field_size.width, &field_size.height);

        ImGui::VecInput(message(Message::Offset), message(Message::LeftEdge), message(Message::TopEdge),
                        &field_size.left, &field_size.top);

        ImGui::NewLine();

        ImGui::SeparatorText(message(Message::ScreenOpts));

        ImGui::Checkbox(message(Message::Fullscreen), &fullscreen);
        ImGui::NewLine();

        ImGui::InputInt2(message(Message::WindowSize), (int*)&size);
    }

    ~MainTabContents() {
        try {
            data["name"] = name;
            data["description"] = description;
            data["fullscreen"] = fullscreen;
            data["field_size"] = field_size;
            // data["fps"] = fps;
            data["bg"] = bg;
            data["size"] = size;
            data["last_updated"] = time(nullptr);
        } catch (std::exception& e) {
#ifdef DEBUG
            LOG(e.what());
#endif
        }
    }

  private:
    std::string name;
    std::string description;
    sf::Vector2i size;
    sf::FloatRect field_size;
    int fps;
    bool fullscreen;
    std::string bg;

    SideMenuOptions menu;

    nl::json& data;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(MainTabContents, name, description, size, field_size, fullscreen, bg)
};
}  // namespace

Menu::Tab MainTab(nl::json& json) {
    auto tab = std::make_unique<MainTabContents>(json);
    nl::from_json(json, *tab);
    return Menu::Tab(std::move(tab), message_func(Message::GameOpts));
}
}  // namespace ui
