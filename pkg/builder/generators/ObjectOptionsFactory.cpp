#include "ObjectOptionsFactory.hpp"

#include <unordered_map>
#include <variant>

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace {
void append_to_res(ObjectOptionsFactory::res_type& res, const nl::json json, ObjectOptions& opts);
}

ObjectOptionsFactory::ObjectOptionsFactory(const GameField& field)
    : handler_chain_(init_handler_chain(field)) {}

ErrorOr<ObjectOptions> ObjectOptionsFactory::generate(const nl::json& json) const {
    ObjectOptions res;

    auto error = handler_chain_.handle(res, json);
    if (error) {
        return tl::unexpected(error);
    }

    if (res.type == "") {
        return unexpected_error<KeyError>("type");
    }

    return res;
}

ErrorOr<ObjectOptionsFactory::res_type> ObjectOptionsFactory::generate(
    const std::vector<nl::json>& json) const {
#ifdef DEBUG
    LOG("Generating object instance options");
#endif
    res_type res;

    for (auto& obj : json) {
        auto opts = generate(obj);
        if (!opts) {
            return tl::unexpected(opts.error());
        }
        append_to_res(res, obj, opts.value());
    }

    return res;
}

namespace {

void append_to_res(ObjectOptionsFactory::res_type& res, const nl::json json, ObjectOptions& opts) {
    if (!json.contains("delta") || !json.contains("count")) {
        return res.push_back(std::move(opts));
    }

    size_t count = json.at("count").template get<size_t>();
    auto delta = json.at("delta").template get<sf::Vector2f>();

    for (size_t i = 0; i < count; ++i) {
        res.push_back(opts);

        opts.pos_x += delta.x;
        opts.pos_y += delta.y;
    }
}

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

class MoveTypeHandler;
class MoveArgsHandler;

struct MoveHandler : Handler<ObjectOptions> {
  public:
    MoveHandler();

    bool should_handle(const std::string& key) const override {
        return key == "move";
    }

    void handle(ObjectOptions& obj, const std::string&, const nl::json& value) override {
        inner_chain_.handle_unsafe(*this, value);

        if (!set_args_) {
            obj.move = movement::no_op;
        }

        obj.move = set_args_(args_);
    }

  private:
    using args_type = std::unordered_map<std::string, std::variant<sf::Vector2f, float>>;

    HandlerChain<MoveHandler> inner_chain_;

    std::function<movement::Func(args_type&)> set_args_;
    args_type args_;

    friend class MoveTypeHandler;
    friend class MoveArgsHandler;
};

struct MoveTypeHandler : public Handler<MoveHandler> {
    bool should_handle(const std::string& key) const override {
        return key == "type";
    }

    void handle(MoveHandler& obj, const std::string&, const nl::json& value) override {
        auto str_value = value.template get<std::string>();

        // clang-format off
        if (str_value == "linear") {
            obj.set_args_ = [](auto& args) {
                return movement::linear(
                    std::get<float>(args["x"]),
                    std::get<float>(args["y"])
                );
            };
        }

        if (str_value == "circular") {
            obj.set_args_ = [](auto& args) {
                return movement::circular(
                    std::get<sf::Vector2f>(args["center"]),
                    std::get<float>(args["speed"])
                );
            };
        }
        // clang-format on
    }
};

struct MoveArgsHandler : public Handler<MoveHandler> {
    bool should_handle(const std::string& key) const override {
        return key == "args";
    }

    void handle(MoveHandler& obj, const std::string&, const nl::json& json) override {
        for (auto& [key, value] : json.items()) {
            if (value.is_number()) {
                obj.args_[key] = value.template get<float>();
                continue;
            }

            obj.args_[key] = value.template get<sf::Vector2f>();
        }
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

MoveHandler::MoveHandler() {
    inner_chain_.add_handler(std::make_unique<MoveTypeHandler>());
    inner_chain_.add_handler(std::make_unique<MoveArgsHandler>());
}

}  // namespace

HandlerChain<ObjectOptions> ObjectOptionsFactory::init_handler_chain(const GameField& field) {
    std::vector<std::unique_ptr<Handler<ObjectOptions>>> res;
    res.reserve(6);

    res.push_back(std::make_unique<TypeHandler>());
    res.push_back(std::make_unique<PosHandler>(field));
    res.push_back(std::make_unique<MoveHandler>());
    res.push_back(std::make_unique<ActivityHandler>(field));
    res.push_back(std::make_unique<RotationHandler>());
    res.push_back(std::make_unique<PropsHandler<ObjectOptions>>());

    return res;
};
