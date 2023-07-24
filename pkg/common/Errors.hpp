#pragma once

#include <format>
#include <ostream>
#include <string>

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

#undef XX

#define XX(error_code, error_message) \
    case Error::Code::error_message:  \
        return #error_message;

    virtual std::string message() const {
        switch (code) {
            ERROR_CODES
            default:
                return "UNKNOWN";
        }
    }

    Error(Code code = Code::Undefined) : code(code) {}

    Code code;
};

struct KeyError : public Error {
  public:
    KeyError(std::string key) : Error(Code::NoKey), key_(std::move(key)) {}

    std::string message() const override {
        return std::format("Key '{}' is required", key_);
    }

  private:
    std::string key_;
};

struct TypeError : public Error {};

struct InternalError : public Error {};

inline std::ostream& operator<<(std::ostream& out, Error error) {
    return out << error.message();
}

#undef XX
#undef ERROR_CODES
