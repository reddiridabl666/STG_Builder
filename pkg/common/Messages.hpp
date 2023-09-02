#pragma once

#include <array>
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
    Exit,
    CreateGame,
    CreateLevel,
    Back,
    UnexpectedError,
    Close,
};

const char* message(Message type);
