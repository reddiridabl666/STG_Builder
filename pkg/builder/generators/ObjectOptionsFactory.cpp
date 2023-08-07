#include "ObjectOptionsFactory.hpp"

#include <unordered_map>
#include <variant>

#ifdef DEBUG
#include "Debug.hpp"
#endif

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
        res.push_back(std::move(opts.value()));
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

struct X_Handler : Handler<ObjectOptions> {
    X_Handler(const GameField& field) : field_(field) {}

    bool should_handle(const std::string& key) const override {
        return key == "x";
    }

    void handle(ObjectOptions& obj, const std::string&, const nl::json& value) override {
        if (!value.is_string()) {
            obj.pos_x = position::set(value.template get<float>())(field_);
            return;
        }

        auto str = value.template get<std::string>();
        if (str.ends_with('%')) {
            int percent = std::stoi(str.substr(0, str.size() - 1));
            obj.pos_x = position::percent_width(percent)(field_);
            return;
        }

        throw std::runtime_error(fmt::format("Got invalid value for X: '{}'", str));
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
        if (!value.is_string()) {
            obj.pos_y = position::set(value.template get<float>())(field_);
            return;
        }

        auto str = value.template get<std::string>();
        if (str.ends_with('%')) {
            int percent = std::stoi(str.substr(0, str.size() - 1));
            obj.pos_y = position::percent_height(percent)(field_);
            return;
        }

        throw std::runtime_error(fmt::format("Got invalid value for Y: '{}'", str));
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
                    std::get<float>(args["radius"])
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

MoveHandler::MoveHandler() {
    inner_chain_.add_handler(std::make_unique<MoveTypeHandler>());
    inner_chain_.add_handler(std::make_unique<MoveArgsHandler>());
}

}  // namespace

HandlerChain<ObjectOptions> ObjectOptionsFactory::init_handler_chain(const GameField& field) {
    std::vector<std::unique_ptr<Handler<ObjectOptions>>> res;
    res.reserve(4);

    res.push_back(std::make_unique<TypeHandler>());
    res.push_back(std::make_unique<PosHandler>(field));
    res.push_back(std::make_unique<MoveHandler>());
    res.push_back(std::make_unique<PropsHandler<ObjectOptions>>());

    return res;
};
