#include "Messages.hpp"

#include <array>
#include <unordered_map>

#define U8(str) reinterpret_cast<const char*>(u8##str)

const char* message(Message type) {
    static std::unordered_map<Message, std::array<const char*, Lang::Count>> messages = {
        {Message::Exit, {U8("Выход"), "Quit"}},
        {Message::Games, {U8("Ваши игры"), "Your games"}},
        {Message::Levels, {U8("Ваши уровни"), "Your levels"}},
        {Message::CreateGame, {U8("Создать игру"), "New Game"}},
        {Message::CreateLevel, {U8("Создать уровень"), "New Level"}},
        {Message::Back, {U8("Назад"), "Back"}},
        {Message::UnexpectedError, {U8("Непредвиденная ошибка"), "Unexpected error"}},
        {Message::Close, {U8("Закрыть"), "Close"}},
        {Message::Name, {U8("Название"), "Name"}},
        {Message::Desc, {U8("Описание"), "Description"}},
        {Message::GameField, {U8("Настройки игрового поля"), "Game field options"}},
        {Message::SideMenu, {U8("Настройки бокового меню"), "Side menu options"}},
        {Message::GameOpts, {U8("Настройки игры"), "Game"}},
        {Message::ScreenOpts, {U8("Настройки экрана"), "Screen options"}},
        {Message::WindowSize, {U8("Размер окна"), "Window size"}},
        {Message::Fullscreen, {U8("Полноэкранный режим"), "Fullscreen"}},
        {Message::FPS, {U8("Кадры в секунду"), "Frames per second"}},
        {Message::GameFieldSize, {U8("Отступы и размер игрового поля"), "Game field offset and size"}},
        {Message::Menu, {U8("Меню"), "Menu"}},
        {Message::LevelOpts, {U8("Настройки уровня"), "Level"}},
    };

    return messages[type][Lang::current()];
}
