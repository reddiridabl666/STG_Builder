#pragma once

#include <iostream>
#include <memory>
#include <vector>

#ifdef DEBUG
#include "Debug.hpp"
#endif

#include "Json.hpp"

template <typename T>
struct Handler {
    virtual bool should_handle(const std::string& key) const = 0;
    virtual void handle(T& obj, const std::string& key, const nl::json& value) = 0;
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
            std::clog << e.what();
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

    ErrorPtr handle(T& res, const nl::json& json) const noexcept {
#ifdef DEBUG
        LOG(fmt::format("Parsing json: {}", json));
#endif
        for (auto& [key, value] : json.items()) {
            auto error = handle(res, key, value);
            if (error) {
                return error;
            }
        }
        return nullptr;
    }

    void handle_unsafe(T& res, const nl::json& json) const {
#ifdef DEBUG
        LOG(fmt::format("Parsing json: {}", json));
#endif
        for (auto& [key, value] : json.items()) {
            auto error = handle(res, key, value);
            if (error) {
                throw std::runtime_error(error.message());
            }
        }
    }

    ErrorPtr handle(T& res, const std::string& key, const nl::json& value) const noexcept {
        for (auto& handler : handlers_) {
            if (handler->should_handle(key)) {
                try {
                    handler->handle(res, key, value);
                    return nullptr;
                } catch (std::exception& e) {
                    return make_error<InternalError>(e.what());
                }
            }
        }
        return nullptr;
    }

  private:
    storage_type handlers_;
};
