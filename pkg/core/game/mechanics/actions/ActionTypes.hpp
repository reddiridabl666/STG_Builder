#pragma once

#include "Action.hpp"
#include "GameObject.hpp"
#include "Getter.hpp"

namespace action {

struct ConstGetter : public PropertyGetter {
  public:
    ConstGetter(float value) : value_(value) {}

    Value operator()(GameObject&) const override {
        return value_;
    }

    nl::json to_json() const override {
        return value_;
    }

  private:
    const float value_;
};

struct ObjectPropertyGetter : public PropertyGetter {
  public:
    ObjectPropertyGetter(std::string&& property) : property_(std::move(property)) {}

    virtual Value operator()(GameObject& subject) const {
        return subject.props().get(property_);
    }

    nl::json to_json() const override {
        return property_;
    }

  private:
    const std::string property_;
};

struct PropertyUpdater : public Action {
  public:
    PropertyUpdater(std::string&& property, std::unique_ptr<PropertyGetter>&& getter)
        : property_(std::move(property)), get_(std::move(getter)) {}

    virtual std::string type() const = 0;

    nl::json to_json() const override {
        return {
            {"type", type()},
            {"property", property_},
            {"value", get_->to_json()},
        };
    }

  protected:
    const std::string property_;
    const std::unique_ptr<PropertyGetter> get_;
};

struct Resetter : public Action {
  public:
    Resetter(std::string&& property) : property_(std::move(property)) {}

    void operator()(GameObject&, GameObject& object) const override {
        object.props().at(property_).reset();
    }

    nl::json to_json() const override {
        return {
            {"type", "reset"},
            {"property", property_},
        };
    }

  private:
    const std::string property_;
};

struct Incrementor : public Action {
  public:
    Incrementor(std::string&& property) : property_(std::move(property)) {}

    void operator()(GameObject&, GameObject& object) const override {
        object.props().at(property_).inc();
    }

    nl::json to_json() const override {
        return {
            {"type", "inc"},
            {"property", property_},
        };
    }

  private:
    const std::string property_;
};

struct Decrementor : public Action {
  public:
    Decrementor(std::string&& property) : property_(std::move(property)) {}

    void operator()(GameObject&, GameObject& object) const override {
        object.props().at(property_).dec();
    }

    nl::json to_json() const override {
        return {
            {"type", "dec"},
            {"property", property_},
        };
    }

  private:
    const std::string property_;
};

struct PropertySetter : public PropertyUpdater {
    using PropertyUpdater::PropertyUpdater;

    void operator()(GameObject& subject, GameObject& object) const override {
        object.props().at(property_).set(get_->operator()(subject));
    }

    std::string type() const override {
        return "set";
    }
};

struct PropertyAdder : public PropertyUpdater {
    using PropertyUpdater::PropertyUpdater;

    void operator()(GameObject& subject, GameObject& object) const override {
        object.props().at(property_).add(get_->operator()(subject));
    }

    std::string type() const override {
        return "add";
    }
};

struct PropertyMultiplier : public PropertyUpdater {
    using PropertyUpdater::PropertyUpdater;

    void operator()(GameObject& subject, GameObject& object) const override {
        object.props().at(property_).mul(get_->operator()(subject));
    }

    std::string type() const override {
        return "mul";
    }
};

#include <iostream>

struct PropertySubber : public PropertyUpdater {
    using PropertyUpdater::PropertyUpdater;

    void operator()(GameObject& subject, GameObject& object) const override {
        object.props().at(property_).sub(get_->operator()(subject));
    }

    std::string type() const override {
        return "sub";
    }
};

struct PropertyDivisor : public PropertyUpdater {
    using PropertyUpdater::PropertyUpdater;

    void operator()(GameObject& subject, GameObject& object) const override {
        object.props().at(property_).div(get_->operator()(subject));
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
