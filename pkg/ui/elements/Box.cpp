#include "Box.hpp"

#include "Common.hpp"
#include "Messages.hpp"

namespace ui {
Box::Box(const VariableText& name, std::vector<std::unique_ptr<Element>>&& elems, const ImVec2& size,
         const ImVec2& pos)
    : Element(pos, size), name_(name), elems_(std::move(elems)) {}

void Box::draw(const Window& window) {
    ImGui::SetNextWindowPos(get_pos(), 0, ImVec2{0.5, 0.5});
    ImGui::SetNextWindowSizeConstraints(ImVec2{get_size().x, 0}, get_size());

    ImGui::Begin(name_(), nullptr, kStaticWindow | ImGuiWindowFlags_AlwaysAutoResize);

    for (auto& elem : elems_) {
        ImGui::PushID(&elem);
        elem->set_size(ImVec2{ImGui::GetContentRegionAvail().x, elem->get_size().y});
        elem->draw(window);
        ImGui::PopID();

        ImGui::NewLine();
    }

    ImGui::End();
}
}  // namespace ui
