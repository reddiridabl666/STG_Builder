#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Common.hpp"
#include "UiElement.hpp"

namespace ui {
template <typename T>
struct Box : public Element {
  public:
    using Items = std::vector<std::unique_ptr<T>>;

    Box(const VariableText& name, Items&& elems, const ImVec2& size, const ImVec2& pos = {});
    Box(Items&& elems, const ImVec2& size);

    void draw(const Window&) override;

    size_t size() const {
        return elems_.size();
    }

    void push_back(Items::value_type&& elem) {
        elems_.push_back(std::move(elem));
    }

    auto& elems() {
        return elems_;
    }

    const auto& elems() const {
        return elems_;
    }

  private:
    const VariableText name_;
    Items elems_;
    const bool is_child_;
};

using DefaultBox = Box<Element>;

template <typename T>
Box<T>::Box(const VariableText& name, Items&& elems, const ImVec2& size, const ImVec2& pos)
    : Element(pos, size), name_(name), elems_(std::move(elems)), is_child_(false) {}

template <typename T>
Box<T>::Box(Items&& elems, const ImVec2& size) : Element({}, size), elems_(std::move(elems)), is_child_(true) {}

template <typename T>
void Box<T>::draw(const Window& window) {
    if (!is_child_) {
        ImGui::SetNextWindowPos(get_pos(), 0, ImVec2{0.5, 0.5});
        ImGui::SetNextWindowSizeConstraints(ImVec2{get_size().x, 0}, get_size());

        ImGui::Begin(name_(), nullptr, kStaticWindow | ImGuiWindowFlags_AlwaysAutoResize);
    }

    for (auto& elem : elems_) {
        ImGui::PushID(&elem);
        elem->set_size(ImVec2{ImGui::GetContentRegionAvail().x, elem->get_size().y});
        elem->draw(window);
        ImGui::PopID();

        ImGui::NewLine();
    }

    if (!is_child_) {
        ImGui::End();
    }
}
}  // namespace ui
