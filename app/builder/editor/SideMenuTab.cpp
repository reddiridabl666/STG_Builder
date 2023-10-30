#include "SideMenuTab.hpp"

#include "ImguiUtils.hpp"
#include "Messages.hpp"
#include "SideMenu.hpp"
#include "ui/common/Bus.hpp"

namespace ui {
namespace {

struct SideMenuTabContents : public Element {
  public:
    SideMenuTabContents(engine::SideMenuProps&& props) : props_(std::move(props)) {}

    void draw(const Window&) override {
        ImGui::BeginGroup();

        ImGui::InputText(message(Message::Background), &props_.bg);

        ImGui::SeparatorText(message(Message::SizeAndOffset));
        ImGui::Text(message(Message::GameFieldHint));

        ImGui::SizeInput(message(Message::Size), &props_.size.width, &props_.size.height);

        ImGui::VecInput(message(Message::Offset), message(Message::LeftEdge), message(Message::TopEdge),
                        &props_.size.left, &props_.size.top);

        ImGui::SeparatorText(message(Message::Layout));

        ImGui::PosInput(message(Message::Offset), &props_.offset.x, &props_.offset.y, "");
        ImGui::NewLine();
        ImGui::InputFloat(message(Message::Gap), &props_.gap);
        ImGui::InputFloat(message(Message::PlayerGap), &props_.player_gap);

        ImGui::EndGroup();

        if (ImGui::IsItemDeactivatedAfterEdit()) {
            Bus<nl::json>::get().emit(Event::SideMenuChanged, props_);
        }
    }

  private:
    engine::SideMenuProps props_;
};
}  // namespace

Menu::Tab SideMenuTab(nl::json& json) {
    auto tab = std::make_unique<SideMenuTabContents>(json.get<engine::SideMenuProps>());
    return Menu::Tab(std::move(tab), message_func(Message::SideMenu));
}
}  // namespace ui
