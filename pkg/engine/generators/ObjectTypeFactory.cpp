#include "ObjectTypeFactory.hpp"

#include "FuncBuilder.hpp"
#include "PatternFactory.hpp"

ObjectTypeFactory::res_type ObjectTypeFactory::generate(const nl::json& json) {
#ifdef DEBUG
    LOG("Generating object types");
#endif

    res_type res;

    for (auto& [key, value] : json.items()) {
        auto obj = generate(key, value);
        res[key] = std::move(obj);
    }

    return res;
}

ObjectType ObjectTypeFactory::generate(const std::string& name, const nl::json& json) {
#ifdef DEBUG
    LOG(fmt::format("Generating object type: {}", name));
#endif

    ObjectType res(name);
    handler_chain_.handle(res, json);
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
        obj.speed = value.template get<Value>();
    }
};

struct TagHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key == "tag";
    }

    void handle(ObjectType& obj, const std::string&, const nl::json& value) override {
        obj.tag = value.get<GameObjectTag>();
    }
};

struct CollisionHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key == "collision";
    }

    void handle(ObjectType& obj, const std::string&, const nl::json& value) override {
        obj.collision = value.get<ObjectType::CollisionAction>();
    }
};

struct LifeHandler : Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key == "lives";
    }

    void handle(ObjectType& obj, const std::string&, const nl::json& value) override {
        auto life_info = value.template get<FuncInfo>();
        obj.life_func = FuncBuilder::generate<alive::update>(life_info);
    }
};

struct HitboxHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key == "hitbox";
    }

    void handle(ObjectType& obj, const std::string&, const nl::json& value) override {
        obj.hitbox_props = value.get<HitboxProps>();
    }
};

struct OnDeathHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key == "on_character_death";
    }

    void handle(ObjectType& obj, const std::string&, const nl::json& value) override {
        value.get_to(obj.on_death);
    }
};

struct TimedActionsHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key == "timed_actions";
    }

    void handle(ObjectType& obj, const std::string&, const nl::json& value) override {
        obj.timed_actions = value;
    }
};

struct OnOwnHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key == "on_own_action";
    }

    void handle(ObjectType& obj, const std::string&, const nl::json& value) override {
        value.get_to(obj.on_own);
    }
};

struct OnPlayerHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key == "on_player_action";
    }

    void handle(ObjectType& obj, const std::string&, const nl::json& value) override {
        value.get_to(obj.on_player);
    }
};

struct PatternHandler : public Handler<ObjectType> {
    bool should_handle(const std::string& key) const override {
        return key == "patterns";
    }

    void handle(ObjectType& obj, const std::string&, const nl::json& json) override {
        for (auto& [key, value] : json.items()) {
            obj.patterns[key] = PatternFactory::create(value);
        }
    }
};
}  // namespace

HandlerChain<ObjectType> ObjectTypeFactory::handler_chain_ = [] {
    std::vector<std::unique_ptr<Handler<ObjectType>>> res;
    res.reserve(14);

    res.push_back(std::make_unique<SizeHandler>());
    res.push_back(std::make_unique<SpeedHandler>());
    res.push_back(std::make_unique<TagHandler>());
    res.push_back(std::make_unique<ImageHandler>());
    res.push_back(std::make_unique<SoundHandler>());

    res.push_back(std::make_unique<LifeHandler>());

    res.push_back(std::make_unique<HitboxHandler>());
    res.push_back(std::make_unique<CollisionHandler>());

    res.push_back(std::make_unique<OnDeathHandler>());
    res.push_back(std::make_unique<OnOwnHandler>());
    res.push_back(std::make_unique<OnPlayerHandler>());
    res.push_back(std::make_unique<TimedActionsHandler>());

    res.push_back(std::make_unique<PatternHandler>());
    res.push_back(std::make_unique<PropsHandler<ObjectType>>());

    return res;
}();
