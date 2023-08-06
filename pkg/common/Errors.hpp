#pragma once

#include <fmt/core.h>

#include <concepts>
#include <memory>
#include <ostream>
#include <string>
#include <tl/expected.hpp>

#define ERROR_CODES    \
    XX(0, OK)          \
    XX(1, NoKey)       \
    XX(2, InvalidType) \
    XX(3, Internal)    \
    XX(4, Undefined)

#define XX(error_code, error_message) error_message = error_code,

struct Error {
    enum class Code {
        ERROR_CODES
    };

    virtual std::string message() const = 0;
};

struct ErrorCode : public Error {
#undef XX

#define XX(error_code, error_message) \
    case Error::Code::error_message:  \
        return #error_message;

    std::string message() const override {
        switch (code) {
            ERROR_CODES
            default:
                return "UNKNOWN";
        }
    }

    ErrorCode(Code code = Code::Undefined) : code(code) {}

    Code code;
};

struct ErrorPtr : public Error {
  public:
    static constexpr nullptr_t OK = nullptr;

    explicit ErrorPtr(std::shared_ptr<Error>&& error) : value_(std::move(error)) {}

    ErrorPtr(nullptr_t null) : value_(null) {}

    std::string message() const override {
        return value_->message();
    }

    operator bool() const {
        return bool(value_);
    }

  private:
    std::shared_ptr<Error> value_;
};

template <typename T>
using ErrorOr = tl::expected<T, ErrorPtr>;

template <typename T, typename... Ts>
inline ErrorPtr make_error(Ts&&... args) {
    return ErrorPtr(std::make_shared<T>(std::forward<Ts>(args)...));
}

template <typename T, typename... Ts>
inline tl::unexpected<ErrorPtr> unexpected_error(Ts&&... args) {
    return tl::unexpected(make_error<T>(std::forward<Ts>(args)...));
}

struct KeyError : public ErrorCode {
  public:
    KeyError(std::string key) : ErrorCode(Code::NoKey), key_(std::move(key)) {}

    std::string message() const override {
        return fmt::format("Key '{}' is required", key_);
    }

  protected:
    std::string key_;
};

struct NoKeyError : public KeyError {
  public:
    NoKeyError(std::string key) : KeyError(std::move(key)) {}

    std::string message() const override {
        return fmt::format("No such key: '{}'", key_);
    }
};

struct TypeError : public ErrorCode {
  public:
    TypeError(std::string key, std::string req, std::string got)
        : ErrorCode(Code::InvalidType), req_(std::move(req)), got_(std::move(got)), key_(std::move(key)) {}

    std::string message() const override {
        return fmt::format("Expected type '{}', but got '{}' at key '{}'", req_, got_, key_);
    }

  private:
    std::string req_;
    std::string got_;
    std::string key_;
};

struct InternalError : public ErrorCode {
  public:
    InternalError(std::string msg) : ErrorCode(Code::Internal), msg_(std::move(msg)) {}

    std::string message() const override {
        return msg_;
    }

  private:
    std::string msg_;
};

inline std::ostream& operator<<(std::ostream& out, const Error& error) {
    return out << error.message();
}

#undef XX
#undef ERROR_CODES
