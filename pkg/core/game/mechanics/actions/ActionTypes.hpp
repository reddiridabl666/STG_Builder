#pragma once

#include "Action.hpp"
#include "GameBus.hpp"
#include "GameObject.hpp"
#include "Getter.hpp"
#include "Random.hpp"

namespace action {

struct ConstGetter : public Getter {
  public:
    ConstGetter(float value) : value_(value) {}

    Value get(const GameObject&) const override {
        return value_;
    }

  private:
    const float value_;
};

struct RandomGetter : public Getter {
  public:
    RandomGetter(size_t left, size_t right) : left_(left), right_(right) {}

    Value get(const GameObject&) const override {
        return rng::get_uint(left_, right_);
    }

  private:
    const size_t left_;
    const size_t right_;
};

struct ObjectPropertyGetter : public MutableGetter {
  public:
    ObjectPropertyGetter(std::string&& property) : property_(std::move(property)) {}

    Value& get(GameObject& subject) override {
        return subject.props().at(property_);
    }

  private:
    const std::string property_;
};

struct SpeedGetter : public MutableGetter {
    Value& get(GameObject& subject) override {
        return subject.speed();
    }
};

struct BinaryAdapter : public BinaryAction {
  public:
    static std::unique_ptr<BinaryAction> wrap(std::unique_ptr<Action>&& action) {
        if (!action) {
            return nullptr;
        }
        return std::make_unique<BinaryAdapter>(std::move(action));
    }

    BinaryAdapter(std::unique_ptr<Action>&& action) : action_(std::move(action)) {}

    void operator()(std::weak_ptr<const GameObject>, std::weak_ptr<GameObject> object) const override {
        action_->operator()(object);
    }

    void operator()(std::weak_ptr<GameObject> object) const override {
        action_->operator()(object);
    }

  private:
    std::unique_ptr<Action> action_;
};

struct Shooter : public Action {
  public:
    Shooter(const std::string& pattern) : pattern_(pattern) {}

    void operator()(std::weak_ptr<GameObject> object) const override {
        nl::json payload{
            {"object", object.lock()->name()},
            {"type", object.lock()->type_name()},
            {"pattern", pattern_},
        };
        GameBus::get().emit(GameEvent::BulletShot, payload);
    }

  private:
    const std::string pattern_;
};

struct Killer : public Action {
    void operator()(std::weak_ptr<GameObject> object) const override {
        object.lock()->die();
    }
};

struct VisibilityToggler : public Action {
    void operator()(std::weak_ptr<GameObject> object) const override {
        object.lock()->toggle_visibility();
    }
};

struct PropertyUpdater : virtual public Action {
  public:
    PropertyUpdater(std::unique_ptr<MutableGetter>&& property) : property_(std::move(property)) {}

  protected:
    const std::unique_ptr<MutableGetter> property_;
};

struct Resetter : public PropertyUpdater {
    using PropertyUpdater::PropertyUpdater;

    void operator()(std::weak_ptr<GameObject> object) const override {
        property_->get(*object.lock()).reset();
    }
};

struct Incrementor : public PropertyUpdater {
    using PropertyUpdater::PropertyUpdater;

    void operator()(std::weak_ptr<GameObject> object) const override {
        property_->get(*object.lock()).inc();
    }
};

struct Decrementor : public PropertyUpdater {
    using PropertyUpdater::PropertyUpdater;

    void operator()(std::weak_ptr<GameObject> object) const override {
        property_->get(*object.lock()).dec();
    }
};

struct PropertyValueUpdater : public PropertyUpdater, public BinaryAction {
  public:
    PropertyValueUpdater(std::unique_ptr<MutableGetter>&& property, std::unique_ptr<Getter>&& value)
        : PropertyUpdater(std::move(property)), value_(std::move(value)) {}

  protected:
    const std::unique_ptr<Getter> value_;
};

struct PropertySetter : public PropertyValueUpdater {
    using PropertyValueUpdater::PropertyValueUpdater;

    void operator()(std::weak_ptr<const GameObject> subject, std::weak_ptr<GameObject> object) const override {
        property_->get(*object.lock()).set(value_->get(*subject.lock()));
    }
};

struct PropertyAdder : public PropertyValueUpdater {
    using PropertyValueUpdater::PropertyValueUpdater;

    void operator()(std::weak_ptr<const GameObject> subject, std::weak_ptr<GameObject> object) const override {
        property_->get(*object.lock()).add(value_->get(*subject.lock()));
    }
};

struct PropertyMultiplier : public PropertyValueUpdater {
    using PropertyValueUpdater::PropertyValueUpdater;

    void operator()(std::weak_ptr<const GameObject> subject, std::weak_ptr<GameObject> object) const override {
        property_->get(*object.lock()).mul(value_->get(*subject.lock()));
    }
};

struct PropertySubber : public PropertyValueUpdater {
    using PropertyValueUpdater::PropertyValueUpdater;

    void operator()(std::weak_ptr<const GameObject> subject, std::weak_ptr<GameObject> object) const override {
        property_->get(*object.lock()).sub(value_->get(*subject.lock()));
    }
};

struct PropertyDivisor : public PropertyValueUpdater {
    using PropertyValueUpdater::PropertyValueUpdater;

    void operator()(std::weak_ptr<const GameObject> subject, std::weak_ptr<GameObject> object) const override {
        property_->get(*object.lock()).div(value_->get(*subject.lock()));
    }
};

template <typename ActionType>
struct MultiAction : public ActionType {
  public:
    MultiAction(std::vector<std::unique_ptr<ActionType>>&& actions) : actions_(std::move(actions)) {}

    void operator()(std::weak_ptr<const GameObject> subject,
                    std::weak_ptr<GameObject> object) const requires std::is_same_v<ActionType, BinaryAction> {
        for (auto& action : actions_) {
            action->operator()(subject, object);
        }
    }

    void operator()(std::weak_ptr<GameObject> object) const override {
        for (auto& action : actions_) {
            action->operator()(object);
        }
    }

  private:
    std::vector<std::unique_ptr<ActionType>> actions_;
};
}  // namespace action
