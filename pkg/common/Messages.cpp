#include "Messages.hpp"

#include <array>
#include <unordered_map>

#define U8(str) reinterpret_cast<const char*>(u8##str)

const char* message(Message type) {
    static std::unordered_map<Message, std::array<const char*, Lang::Count>> messages = {
        {Message::Exit, {U8("Выход"), "Quit"}},
        {Message::Games, {U8("Ваши игры"), "Your games"}},
        {Message::CreateGame, {U8("Создать игру"), "New Game"}},
        {Message::CreateLevel, {U8("Создать уровень"), "New Level"}},
        {Message::Back, {U8("Назад"), "Back"}},
        {Message::UnexpectedError, {U8("Непредвиденная ошибка"), "Unexpected error"}},
        {Message::Close, {U8("Закрыть"), "Close"}},

    };

    return messages[type][Lang::current()];
}
