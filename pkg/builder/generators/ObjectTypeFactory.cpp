#include "ObjectTypeFactory.hpp"

ErrorOr<ObjectTypeFactory::res_type> ObjectTypeFactory::generate(const nl::json& json) const {
#ifdef DEBUG
    LOG("Generating object types");
#endif

    res_type res;

    for (auto& [key, value] : json.items()) {
        auto obj = generate(key, value);
        if (!obj) {
            return tl::unexpected(obj.error());
        }
        res[key] = std::move(obj.value());
    }

    return res;
}

ErrorOr<ObjectType> ObjectTypeFactory::generate(const std::string& name, const nl::json& json) const {
#ifdef DEBUG
    LOG(fmt::format("Generating object type: {}", name));
#endif

    ObjectType res(name);

    auto error = handler_chain_.handle(res, json);
    if (error) {
        return tl::unexpected(error);
    }

    return res;
}

namespace {
struct SizeHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key == "size";
    }

    void handle(ObjectType& obj, const std::string&, const nl::json& value) override {
        obj.size = value.template get<sf::Vector2f>();
    }
};

struct ImageHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key.starts_with("image");
    }

    void handle(ObjectType& obj, const std::string& key, const nl::json& value) override {
        if (key == "image") {
            obj.images = {value.template get<std::string>()};
        } else {
            obj.images = value.template get<std::vector<std::string>>();
        }
    }
};

struct SoundHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key.starts_with("sound");
    }

    void handle(ObjectType& obj, const std::string& key, const nl::json& value) override {
        if (key == "sound") {
            obj.sounds = {value.template get<std::string>()};
        } else {
            obj.sounds = value.template get<std::vector<std::string>>();
        }
    }
};

struct SpeedHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key == "speed";
    }

    void handle(ObjectType& obj, const std::string&, const nl::json& value) override {
        obj.speed = value.template get<int>();
    }
};
}  // namespace

HandlerChain<ObjectType> ObjectTypeFactory::handler_chain_ = [] {
    std::vector<std::unique_ptr<Handler<ObjectType>>> res;
    res.reserve(5);

    res.push_back(std::make_unique<SizeHandler>());
    res.push_back(std::make_unique<SpeedHandler>());
    res.push_back(std::make_unique<ImageHandler>());
    res.push_back(std::make_unique<SoundHandler>());
    res.push_back(std::make_unique<PropsHandler<ObjectType>>());

    return res;
}();
