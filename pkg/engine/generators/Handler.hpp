#pragma once

#include <iostream>
#include <memory>
#include <vector>

#ifdef DEBUG
#include "Debug.hpp"
#endif

#include "Json.hpp"
#include "Utils.hpp"

template <typename T>
struct Handler {
    virtual bool should_handle(const std::string& key) const = 0;
    virtual void handle(T& obj, const std::string& key, const nl::json& value) = 0;
    virtual ~Handler() = default;
};

template <typename T>
struct PropsHandler : public Handler<T> {
    bool should_handle(const std::string&) const override {
        return true;
    }

    void handle(T& obj, const std::string& key, const nl::json& value) override {
        try {
            obj.props.set(key, value.template get<int>());
        } catch (nl::json::type_error& e) {
#ifdef DEBUG
            LOG("In key '" + key + "': " + e.what());
#endif
        }
    }
};

template <typename T>
class HandlerChain {
  public:
    using storage_type = std::vector<std::unique_ptr<Handler<T>>>;

    HandlerChain() = default;

    HandlerChain(storage_type&& handlers) : handlers_(std::move(handlers)) {}

    void add_handler(std::unique_ptr<Handler<T>>&& handler) {
        handlers_.push_back(std::move(handler));
    }

    // void set_fallback_handler(std::unique_ptr<Handler<T>>&& handler) {
    //     fallback_ = std::move(handler);
    // }

    Error handle(T& res, const nl::json& json) const noexcept {
#ifdef DEBUG
        LOG(fmt::format("Parsing json: \n{}", json.dump(4)));
#endif
        for (auto& [key, value] : json.items()) {
            auto error = handle(res, key, value);
            if (error) {
                return error;
            }
        }
        return Error::OK;
    }

    void handle_unsafe(T& res, const nl::json& json) const {
#ifdef DEBUG
        LOG(fmt::format("Parsing json: {}", json.dump(4)));
#endif
        for (auto& [key, value] : json.items()) {
            auto error = handle(res, key, value);
            if (error) {
                throw std::runtime_error(error.message());
            }
        }
    }

    Error handle(T& res, const std::string& key, const nl::json& value) const noexcept {
        for (auto& handler : handlers_) {
            if (handler->should_handle(key)) {
                try {
                    handler->handle(res, key, value);
                    return Error::OK;
                } catch (std::exception& e) {
                    auto msg = fmt::format("While handling '{}': '{}', got error: {}", key, to_string(value),
                                           e.what());
                    return Error(msg);
                }
            }
        }

        // if (fallback_) {
        //     fallback_->handle(res, key, value)
        // };
        return Error::OK;
    }

  private:
    storage_type handlers_;
    // storage_type::value_type fallback_;
};
