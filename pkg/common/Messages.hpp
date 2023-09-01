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
    Create
};

const char* message(Message type);

// const char* message(Message type) {
//     static std::unordered_map<Message, std::array<const char*, Lang::Count>> messages = {
//         {Message::Exit, {"Выход", "Quit"}},
//         {Message::Games, {"Ваши игры", "Your games"}},
//         {Message::Create, {"Создать", "Create"}},
//     };

//     return messages[type][Lang::current()];
// }
