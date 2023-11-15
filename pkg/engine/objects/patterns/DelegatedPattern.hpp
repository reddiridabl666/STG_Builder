#pragma once

#include "ObjectType.hpp"
#include "Pattern.hpp"

class DelegatedPattern : public Pattern {
  public:
    DelegatedPattern(std::string&& bullet_type, std::unique_ptr<ObjectCount>&& count,
                     std::unique_ptr<MovementSetter>&& movement, std::unique_ptr<PositionSetter>&& pos)
        : bullet_type_(std::move(bullet_type)),
          count_(std::move(count)),
          movement_(std::move(movement)),
          pos_(std::move(pos)) {}

    objects create(const std::shared_ptr<GameObject>& parent, object_types& types, assets::Manager& assets) override {
        objects res;
        res.reserve(count_->get());
        auto& bullet_type = types.at(bullet_type_);

        for (size_t i = 0; i < res.capacity(); ++i) {
            res.push_back(bullet_type.create_object(ObjectOptions{}, assets));
            res.back()->set_parent(parent);
        }

        pos_->set(*parent, res);
        // movement_->set(res);
        return res;
    }

  protected:
    std::string bullet_type_;

    std::unique_ptr<ObjectCount> count_;
    std::unique_ptr<MovementSetter> movement_;
    std::unique_ptr<PositionSetter> pos_;
};
