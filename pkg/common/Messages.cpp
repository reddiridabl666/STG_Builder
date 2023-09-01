#include "Messages.hpp"

#include <array>
#include <unordered_map>

const char* message(Message type) {
    static std::unordered_map<Message, std::array<const char*, Lang::Count>> messages = {
        {Message::Exit, {"Выход", "Quit"}},
        {Message::Games, {"Ваши игры", "Your games"}},
        {Message::Create, {"Создать", "Create"}},
    };

    return messages[type][Lang::current()];
}
