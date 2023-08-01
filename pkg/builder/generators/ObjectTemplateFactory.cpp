#include "ObjectTemplateFactory.hpp"

tl::expected<ObjectTemplate, ErrorPtr> ObjectTemplateFactory::generate(const std::string& name,
                                                                       const nl::json& json) const {
    ObjectTemplate res(name);

    auto ok = handler_chain_.handle(res, json);
    if (!ok) {
        return tl::unexpected(ok.error());
    }

    return res;
}

namespace {
struct SizeHandler : public Handler<ObjectTemplate> {
    bool should_handle(const std::string& key) const override {
        return key == "size";
    }

    void handle(ObjectTemplate& obj, const std::string& key, const nl::json& value) override {
        obj.size = value.template get<sf::Vector2f>();
    }
};

struct ImageHandler : public Handler<ObjectTemplate> {
    bool should_handle(const std::string& key) const override {
        return key.starts_with("image");
    }

    void handle(ObjectTemplate& obj, const std::string& key, const nl::json& value) override {
        if (key == "image") {
            obj.images = {value.template get<std::string>()};
        } else {
            obj.images = value.template get<std::vector<std::string>>();
        }
    }
};

struct SoundHandler : public Handler<ObjectTemplate> {
    bool should_handle(const std::string& key) const override {
        return key.starts_with("sound");
    }

    void handle(ObjectTemplate& obj, const std::string& key, const nl::json& value) override {
        if (key == "sound") {
            obj.sounds = {value.template get<std::string>()};
        } else {
            obj.sounds = value.template get<std::vector<std::string>>();
        }
    }
};

struct SpeedHandler : public Handler<ObjectTemplate> {
    bool should_handle(const std::string& key) const override {
        return key == "speed";
    }

    void handle(ObjectTemplate& obj, const std::string& key, const nl::json& value) override {
        obj.speed = value.template get<int>();
    }
};
}  // namespace

// clang-format off
HandlerChain<ObjectTemplate> ObjectTemplateFactory::handler_chain_({
    std::make_unique<SizeHandler>(),
    std::make_unique<SpeedHandler>(),
    std::make_unique<ImageHandler>(),
    std::make_unique<SoundHandler>(),
    std::make_unique<PropsHandler<ObjectTemplate>>()
});
// clang-format on
