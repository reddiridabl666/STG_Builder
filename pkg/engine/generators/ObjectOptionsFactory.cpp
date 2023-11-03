#include "ObjectOptionsFactory.hpp"

#include "FuncBuilder.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

ObjectOptionsFactory::ObjectOptionsFactory(const GameField& field) : handler_chain_(init_handler_chain(field)) {}

ObjectOptions ObjectOptionsFactory::generate(const nl::json& json) const {
    ObjectOptions res;

    handler_chain_.handle(res, json);

    if (res.type == "") {
        throw Error::NoKey("type");
    }

    return res;
}

ObjectOptionsFactory::res_type ObjectOptionsFactory::generate(const std::vector<nl::json>& json) const {
#ifdef DEBUG
    LOG("Generating object instance options");
#endif
    res_type res;
    size_t idx = 0;

    for (auto& obj : json) {
        auto opts = generate(obj);
        opts.json_id = idx;
        res.push_back(std::move(opts));
        ++idx;
    }

    return res;
}

namespace {
struct TypeHandler : Handler<ObjectOptions> {
    bool should_handle(const std::string& key) const override {
        return key == "type";
    }

    void handle(ObjectOptions& obj, const std::string&, const nl::json& value) override {
        obj.type = value.template get<std::string>();
    }
};

struct RotationHandler : Handler<ObjectOptions> {
    bool should_handle(const std::string& key) const override {
        return key == "rotation";
    }

    void handle(ObjectOptions& obj, const std::string&, const nl::json& value) override {
        obj.rotation = value.template get<float>();
    }
};

float pos_x(const GameField& field, const nl::json& value) {
    if (!value.is_string()) {
        return position::set(value.template get<float>())(field);
    }

    auto str = value.template get<std::string>();
    if (str.ends_with('%')) {
        int percent = std::stoi(str.substr(0, str.size() - 1));
        return position::percent_width(percent)(field);
    }

    throw std::runtime_error(fmt::format("Got invalid value for X: '{}'", str));
}

float pos_y(const GameField& field, const nl::json& value) {
    if (!value.is_string()) {
        return position::set(value.template get<float>())(field);
    }

    auto str = value.template get<std::string>();
    if (str.ends_with('%')) {
        int percent = std::stoi(str.substr(0, str.size() - 1));
        return position::percent_height(percent)(field);
    }

    throw std::runtime_error(fmt::format("Got invalid value for Y: '{}'", str));
}

struct X_Handler : Handler<ObjectOptions> {
    X_Handler(const GameField& field) : field_(field) {}

    bool should_handle(const std::string& key) const override {
        return key == "x";
    }

    void handle(ObjectOptions& obj, const std::string&, const nl::json& value) override {
        obj.pos_x = pos_x(field_, value);
    }

  private:
    const GameField& field_;
};

struct Y_Handler : Handler<ObjectOptions> {
    Y_Handler(const GameField& field) : field_(field) {}

    bool should_handle(const std::string& key) const override {
        return key == "y";
    }

    void handle(ObjectOptions& obj, const std::string&, const nl::json& value) override {
        obj.pos_y = pos_y(field_, value);
    }

  private:
    const GameField& field_;
};

struct PosHandler : Handler<ObjectOptions> {
  public:
    PosHandler(const GameField& field) {
        inner_chain_.add_handler(std::make_unique<X_Handler>(field));
        inner_chain_.add_handler(std::make_unique<Y_Handler>(field));
    }

    bool should_handle(const std::string& key) const override {
        return key == "pos";
    }

    void handle(ObjectOptions& obj, const std::string&, const nl::json& value) override {
        inner_chain_.handle_unsafe(obj, value);
    }

  private:
    HandlerChain<ObjectOptions> inner_chain_;
};

struct MoveHandler : Handler<ObjectOptions> {
    bool should_handle(const std::string& key) const override {
        return key == "move";
    }

    void handle(ObjectOptions& obj, const std::string&, const nl::json& value) override {
        if (!value.contains("rules")) {
            auto move_info = value.template get<FuncInfo>();
            obj.move = FuncBuilder::generate<std::unique_ptr<movement::Rule>>(move_info);
            return;
        }
        obj.move = FuncBuilder::generate(value.template get<movement::MultiInfo>());
    }
};

struct LifeHandler : Handler<ObjectOptions> {
    bool should_handle(const std::string& key) const override {
        return key == "lives";
    }

    void handle(ObjectOptions& obj, const std::string&, const nl::json& value) override {
        auto life_info = value.template get<FuncInfo>();
        obj.life_func = FuncBuilder::generate<alive::update>(life_info);
    }
};

struct ActivityHandler : public Handler<ObjectOptions> {
  public:
    ActivityHandler(const GameField& field) : field_(field) {}

    bool should_handle(const std::string& key) const override {
        return key == "activity_start";
    }

    void handle(ObjectOptions& obj, const std::string&, const nl::json& json) override {
        obj.activity_start = pos_y(field_, json);
    }

  private:
    const GameField& field_;
};

struct BoundsConditionHandler : public Handler<ObjectOptions> {
  public:
    bool should_handle(const std::string& key) const override {
        return key == "stop_at_bounds";
    }

    void handle(ObjectOptions& obj, const std::string&, const nl::json& json) override {
        obj.stop_at_bounds = json.get<bool>();
    }
};

}  // namespace

HandlerChain<ObjectOptions> ObjectOptionsFactory::init_handler_chain(const GameField& field) {
    std::vector<std::unique_ptr<Handler<ObjectOptions>>> res;
    res.reserve(8);

    res.push_back(std::make_unique<TypeHandler>());
    res.push_back(std::make_unique<PosHandler>(field));
    res.push_back(std::make_unique<MoveHandler>());
    res.push_back(std::make_unique<LifeHandler>());
    res.push_back(std::make_unique<ActivityHandler>(field));
    res.push_back(std::make_unique<RotationHandler>());
    res.push_back(std::make_unique<BoundsConditionHandler>());
    res.push_back(std::make_unique<PropsHandler<ObjectOptions>>());

    return res;
};
