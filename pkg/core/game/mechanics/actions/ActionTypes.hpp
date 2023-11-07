#pragma once

#include "Action.hpp"
#include "GameObject.hpp"
#include "Getter.hpp"

namespace action {

struct ConstGetter : public Getter {
  public:
    ConstGetter(float value) : value_(value) {}

    Value get(GameObject&) const override {
        return value_;
    }

    nl::json to_json() const override {
        return value_;
    }

  private:
    const float value_;
};

struct ObjectPropertyGetter : public MutableGetter {
  public:
    ObjectPropertyGetter(std::string&& property) : property_(std::move(property)) {}

    Value& get(GameObject& subject) override {
        return subject.props().at(property_);
    }

    nl::json to_json() const override {
        return property_;
    }

  private:
    const std::string property_;
};

struct SpeedGetter : public MutableGetter {
    Value& get(GameObject& subject) override {
        return subject.speed();
    }

    nl::json to_json() const override {
        return "speed";
    }
};

struct PropertyUpdater : public Action {
  public:
    PropertyUpdater(std::unique_ptr<MutableGetter>&& property) : property_(std::move(property)) {}

    virtual std::string type() const = 0;

    nl::json to_json() const override {
        return {
            {"type", type()},
            {"property", property_->to_json()},
        };
    }

  protected:
    const std::unique_ptr<MutableGetter> property_;
};

struct Resetter : public PropertyUpdater {
    using PropertyUpdater::PropertyUpdater;

    void operator()(GameObject&, GameObject& object) const override {
        property_->get(object).reset();
    }

    std::string type() const override {
        return "reset";
    }
};

struct Incrementor : public PropertyUpdater {
    using PropertyUpdater::PropertyUpdater;

    void operator()(GameObject&, GameObject& object) const override {
        property_->get(object).inc();
    }

    std::string type() const override {
        return "inc";
    }
};

struct Decrementor : public PropertyUpdater {
    using PropertyUpdater::PropertyUpdater;

    void operator()(GameObject&, GameObject& object) const override {
        property_->get(object).dec();
    }

    std::string type() const override {
        return "dec";
    }
};

struct PropertyValueUpdater : public PropertyUpdater {
  public:
    PropertyValueUpdater(std::unique_ptr<MutableGetter>&& property, std::unique_ptr<Getter>&& value)
        : PropertyUpdater(std::move(property)), value_(std::move(value)) {}

    nl::json to_json() const override {
        return {
            {"type", type()},
            {"property", property_->to_json()},
            {"value", value_->to_json()},
        };
    }

  protected:
    const std::unique_ptr<Getter> value_;
};

struct PropertySetter : public PropertyValueUpdater {
    using PropertyValueUpdater::PropertyValueUpdater;

    void operator()(GameObject& subject, GameObject& object) const override {
        property_->get(object).set(value_->get(subject));
    }

    std::string type() const override {
        return "set";
    }
};

struct PropertyAdder : public PropertyValueUpdater {
    using PropertyValueUpdater::PropertyValueUpdater;

    void operator()(GameObject& subject, GameObject& object) const override {
        property_->get(object).add(value_->get(subject));
    }

    std::string type() const override {
        return "add";
    }
};

struct PropertyMultiplier : public PropertyValueUpdater {
    using PropertyValueUpdater::PropertyValueUpdater;

    void operator()(GameObject& subject, GameObject& object) const override {
        property_->get(object).mul(value_->get(subject));
    }

    std::string type() const override {
        return "mul";
    }
};

struct PropertySubber : public PropertyValueUpdater {
    using PropertyValueUpdater::PropertyValueUpdater;

    void operator()(GameObject& subject, GameObject& object) const override {
        property_->get(object).sub(value_->get(subject));
    }

    std::string type() const override {
        return "sub";
    }
};

struct PropertyDivisor : public PropertyValueUpdater {
    using PropertyValueUpdater::PropertyValueUpdater;

    void operator()(GameObject& subject, GameObject& object) const override {
        property_->get(object).div(value_->get(subject));
    }

    std::string type() const override {
        return "div";
    }
};

struct MultiAction : public Action {
  public:
    MultiAction(std::vector<std::unique_ptr<Action>>&& actions) : actions_(std::move(actions)) {}

    void operator()(GameObject& subject, GameObject& object) const override {
        for (auto& action : actions_) {
            action->operator()(subject, object);
        }
    }

    nl::json to_json() const override {
        nl::json res;
        for (auto& action : actions_) {
            res.push_back(action->to_json());
        }
        return res;
    }

  private:
    std::vector<std::unique_ptr<Action>> actions_;
};
}  // namespace action
