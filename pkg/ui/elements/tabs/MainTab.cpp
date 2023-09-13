#include "MainTab.hpp"

#include "ImguiUtils.hpp"
#include "Json.hpp"
#include "Messages.hpp"

namespace ui {

namespace {
struct SideMenuOptions {
    sf::FloatRect size;
    std::vector<std::pair<std::string, float>> stats;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SideMenuOptions, size, stats);

struct MainTabContents : public Element {
  public:
    std::string name;
    std::string description;
    sf::FloatRect game_field_size;
    sf::Vector2f size;

    SideMenuOptions menu;

    void draw(const Window&) override {
        ImGui::BeginGroup();
        ImGui::InputLeftLabel(message(Message::Name), &name);
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            data["name"] = name;
        }

        ImGui::InputLeftLabelMultiline(message(Message::Desc), &description);
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            data["description"] = description;
        }

        ImGui::Text(message(Message::GameField));
        ImGui::EndGroup();
    }

  private:
    nl::json& data;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MainTabContents, name, description, game_field_size, size, menu);
}  // namespace

Menu::Tab MainTab(nl::json& json) {
    auto tab = std::make_unique<MainTabContents>();

    tab->name = json.at("name").template get<std::string>();
    tab->description = json.at("description").template get<std::string>();

    return Menu::Tab(std::move(tab), message_func(Message::GameOpts));
}
}  // namespace ui
