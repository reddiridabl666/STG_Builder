#include "ObjectTemplateFactory.hpp"

namespace {
struct Handler {
    virtual bool should_handle(const std::string& key) const = 0;
    virtual void handle(ObjectTemplate& obj, const std::string& key, const nl::json& value) const = 0;
};
}  // namespace

tl::expected<ObjectTemplate, ErrorPtr> ObjectTemplateFactory::generate(const std::string& name,
                                                                       nl::json json) const {
    ObjectTemplate res(name);

    std::vector<Handler> handlers;

    for (auto& [key, value] : json.items()) {
        for (auto& handler : handlers) {
            if (handler.should_handle(key)) {
                try {
                    handler.handle(res, key, value);
                } catch (nl::json::exception& e) {
                    return unexpected_error<InternalError>(e.what());
                }
            }
        }
    }

    return res;
}

namespace {
struct SizeHandler : public Handler {
    bool should_handle(const std::string& key) const override {
        return key == "size";
    }

    void handle(ObjectTemplate& obj, const std::string& key, const nl::json& value) const override {
        obj.size = value.template get<sf::Vector2f>();
    }
};

struct ImageHandler : public Handler {
    bool should_handle(const std::string& key) const override {
        return key.starts_with("image");
    }

    void handle(ObjectTemplate& obj, const std::string& key, const nl::json& value) const override {
        if (key == "image") {
            obj.images = {value.template get<std::string>()};
        } else {
            obj.images = value.template get<std::vector<std::string>>();
        }
    }
};

struct SoundHandler : public Handler {
    bool should_handle(const std::string& key) const override {
        return key.starts_with("sound");
    }

    void handle(ObjectTemplate& obj, const std::string& key, const nl::json& value) const override {
        if (key == "sound") {
            obj.sounds = {value.template get<std::string>()};
        } else {
            obj.sounds = value.template get<std::vector<std::string>>();
        }
    }
};

struct SpeedHandler : public Handler {
    bool should_handle(const std::string& key) const override {
        return key == "speed";
    }

    void handle(ObjectTemplate& obj, const std::string& key, const nl::json& value) const override {
        obj.speed = value.template get<int>();
    }
};

struct PropsHandler : public Handler {
    bool should_handle(const std::string& _) const override {
        return true;
    }

    void handle(ObjectTemplate& obj, const std::string& key, const nl::json& value) const override {
        obj.props.set(key, value.template get<int>());
    }
};
}  // namespace
