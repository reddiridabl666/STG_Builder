#pragma once

#include <array>
#include <functional>
#include <string>
#include <unordered_map>

class Lang {
  public:
    enum Value {
        RU,
        EN,
        Count
    };

    static void set(Value lang) {
        current_lang_ = lang;
    }

    static Lang::Value current() {
        return current_lang_;
    }

  private:
    static inline Value current_lang_ = Value::RU;
};

enum class Message {
    Games,
    Levels,
    Exit,
    CreateGame,
    CreateLevel,
    Back,
    UnexpectedError,
    Close,
    Name,
    Desc,
    GameField,
    SideMenu,
    GameOpts,
    Menu,
};

const char* message(Message type);

inline std::function<const char*()> message_func(Message type) {
    return std::bind(message, type);
}
