#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "RTree.hpp"
#include "Window.hpp"

template <typename index_type = std::string>
class DrawableRTree : public RTree<index_type, FloatBox> {
  public:
    void remove(const std::pair<FloatBox, index_type>& value) override {
        RTree<index_type, FloatBox>::remove(value);
        boxes_.erase(value.second);
    }

    void insert(const std::pair<FloatBox, index_type>& value) override {
        RTree<index_type, FloatBox>::insert(value);
        sf::RectangleShape shape(value.first.size());

        shape.setOutlineColor(sf::Color::Green);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(2);
        shape.setPosition(value.first.top_left());

        boxes_.emplace(value.second, shape);
    }

    void draw(Window& window) {
        for (auto& [_, box] : boxes_) {
            window.draw(box);
        }
    }

  private:
    std::unordered_map<index_type, sf::RectangleShape> boxes_;
};
