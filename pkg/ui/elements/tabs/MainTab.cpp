#include "MainTab.hpp"

#include <iostream>

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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SideMenuOptions, size, stats);

struct MainTabContents : public Element {
  public:
    MainTabContents(nl::json& data) : data(data) {}

    std::string name;
    std::string description;
    sf::Vector2i size;
    sf::FloatRect field_size;
    int fps;
    bool is_fullscreen;

    SideMenuOptions menu;

    void draw(const Window&) override {
        ImGui::InputText(message(Message::Name), &name);
        ImGui::InputTextMultiline(message(Message::Desc), &description);
        ImGui::NewLine();

        ImGui::SeparatorText(message(Message::GameField));
        ImGui::Text(message(Message::GameFieldHint));

        ImGui::SizeInput(message(Message::Size), &field_size.width, &field_size.height);

        ImGui::VecInput(message(Message::Offset), message(Message::LeftEdge), message(Message::TopEdge),
                        &field_size.left, &field_size.top);

        ImGui::NewLine();

        ImGui::SeparatorText(message(Message::ScreenOpts));

        // ImGui::Text(message(Message::FPS));
        // ImGui::RadioButton("30", &fps, 30);
        // ImGui::RadioButton("60", &fps, 60);
        // ImGui::RadioButton("120", &fps, 120);
        // ImGui::NewLine();

        ImGui::Checkbox(message(Message::Fullscreen), &is_fullscreen);
        ImGui::NewLine();

        ImGui::InputInt2(message(Message::WindowSize), (int*)&size);
    }

    ~MainTabContents() {
        try {
            data["name"] = name;
            data["description"] = description;
            data["fullscreen"] = is_fullscreen;
            data["field_size"] = field_size;
            // data["fps"] = fps;
            data["size"] = size;
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

// int normalize_fps(int fps) {
//     if (fps < 30) {
//         return 30;
//     }

//     if (fps > 60 && fps != 120) {
//         return 120;
//     }

//     return 60;
// }
}  // namespace

Menu::Tab MainTab(nl::json& json) {
    auto tab = std::make_unique<MainTabContents>(json);

    try {
        tab->name = json.at("name").template get<std::string>();
        tab->description = json.at("description").template get<std::string>();
        tab->is_fullscreen = json.at("fullscreen").template get<bool>();
        // tab->fps = normalize_fps(json.at("fps").template get<int>());
        tab->size = json.at("size").template get<sf::Vector2i>();
        tab->field_size = json.at("field_size").template get<sf::FloatRect>();
    } catch (...) {
    }

    return Menu::Tab(std::move(tab), message_func(Message::GameOpts));
}

}  // namespace ui
