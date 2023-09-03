#pragma once

#include <fmt/core.h>

#include <memory>
#include <ostream>
#include <string>
#include <tl/expected.hpp>

struct Error {
  public:
    static Error OK;

    static inline tl::unexpected<Error> New(const std::string& msg) {
        return tl::unexpected(Error(msg));
    }

    static inline tl::unexpected<Error> NoKey(const std::string& key) {
        return tl::unexpected(Error(fmt::format("No such key: {}", key)));
    }

    Error() : ok_(true) {}

    explicit Error(const std::string& msg) : msg_(msg), ok_(false) {}

    const std::string& message() const {
        return msg_;
    }

    operator bool() const {
        return !ok_;
    }

  private:
    std::string msg_;
    bool ok_;
};

template <typename T>
using ErrorOr = tl::expected<T, Error>;

inline std::ostream& operator<<(std::ostream& out, const Error& error) {
    return out << error.message();
}
