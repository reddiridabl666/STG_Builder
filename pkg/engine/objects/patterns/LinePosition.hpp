#pragma once

#include <SFML/System/Vector2.hpp>

#include "GameObject.hpp"
#include "Pattern.hpp"

class LinePosition : public Pattern::PositionSetter {
  public:
    LinePosition(const sf::Vector2f& parent_offset, const sf::Vector2f& sibling_offset)
        : parent_offset_(parent_offset), sibling_offset_(sibling_offset) {}

    void set(const GameObject& parent, Pattern::objects& objects) const override {
        sf::Vector2f init_pos = parent.pos() + parent_offset_;

        if (objects.size() == 1) {
            objects.front()->set_pos(init_pos);
            return;
        }

        sf::Vector2f cur_pos = [&objects, &init_pos, this] {
            if (objects.size() % 2) {
                return init_pos - sibling_offset_;
            }
            return init_pos - sibling_offset_ / 2;
        }();

        for (size_t i = 0; i < objects.size() / 2; ++i) {
            objects[i]->set_pos(cur_pos);
            cur_pos -= sibling_offset_;
        }

        cur_pos = [&objects, &init_pos, this] {
            if (objects.size() % 2) {
                return init_pos;
            }
            return init_pos + sibling_offset_ / 2;
        }();

        for (size_t i = objects.size() / 2; i < objects.size(); ++i) {
            objects[i]->set_pos(cur_pos);
            cur_pos += sibling_offset_;
        }
    }

  private:
    const sf::Vector2f parent_offset_;
    const sf::Vector2f sibling_offset_;
};
