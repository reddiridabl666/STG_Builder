#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "Json.hpp"

template <typename T>
struct Handler {
    virtual bool should_handle(const std::string& key) const = 0;
    virtual void handle(T& obj, const std::string& key, const nl::json& value = 0);
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

    HandlerChain(storage_type&& handlers) : handlers_(std::move(handlers)) {}

    ErrorOr<void> handle(T& res, const nl::json& json) const noexcept {
        for (auto& [key, value] : json.items()) {
            auto ok = handle(res, key, value);
            if (!ok) {
                return tl::unexpected(ok.error());
            }
        }
        return void_t;
    }

    void handle_unsafe(T& res, const nl::json& json) const {
        for (auto& [key, value] : json.items()) {
            auto ok = handle(res, key, value);
            if (!ok) {
                throw std::runtime_error(ok.error().message());
            }
        }
    }

    ErrorOr<void> handle(T& res, const std::string& key, const nl::json& value) const noexcept {
        for (auto& handler : handlers_) {
            if (handler->should_handle(key)) {
                try {
                    handler->handle(res, key, value);
                    return void;
                } catch (std::exception& e) {
                    return unexpected_error<InternalError>(e.what());
                }
            }
        }
    }

  private:
    storage_type handlers_;
};
