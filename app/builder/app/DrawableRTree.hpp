#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "RTree.hpp"
#include "Window.hpp"

template <typename index_type = std::string>
class DrawableRTree : public RTree<index_type, FloatBox> {
  public:
    void remove(const index_type& index, const FloatBox& box) override {
        RTree<index_type, FloatBox>::remove(index, box);
        boxes_.erase(index);
    }

    void insert(const index_type& index, const FloatBox& box) override {
        RTree<index_type, FloatBox>::insert(index, box);
        sf::RectangleShape shape(box.size());

        shape.setOutlineColor(sf::Color::Green);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(2);
        shape.setPosition(box.top_left());

        boxes_.emplace(index, shape);
    }

    void draw(Window& window) {
        for (auto& [_, box] : boxes_) {
            window.draw(box);
        }
    }

  private:
    std::unordered_map<index_type, sf::RectangleShape> boxes_;
};
