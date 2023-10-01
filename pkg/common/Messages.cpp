#include "Messages.hpp"

#include <array>
#include <unordered_map>

#define U8(str) reinterpret_cast<const char*>(u8##str)

const char* message(Message type) {
    static const std::unordered_map<Message, std::array<const char*, Lang::Count>> messages = {
        {Message::Exit, {U8("Выход"), "Quit"}},
        {Message::Games, {U8("Ваши игры"), "Your games"}},
        {Message::YourLevels, {U8("Ваши уровни"), "Your levels"}},
        {Message::Delete, {U8("Удалить"), "Delete"}},
        {Message::CreateGame, {U8("Создать игру"), "New Game"}},
        {Message::CreateLevel, {U8("Создать уровень"), "New Level"}},
        {Message::CreateObjectType, {U8("Новый тип объекта"), "New object type"}},
        {Message::NewObject, {U8("Создать новый объект"), "New object"}},
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
        {Message::Speed, {U8("Скорость"), "Speed"}},
        {Message::Image, {U8("Изображение"), "Image"}},
        {Message::Offset, {U8("Отступы"), "Offset"}},
        {Message::Size, {U8("Размер"), "Size"}},
        {Message::LeftEdge, {U8("Слева"), "Left"}},
        {Message::TopEdge, {U8("Сверху"), "Top"}},
        {Message::Height, {U8("Высота"), "Height"}},
        {Message::Width, {U8("Ширина"), "Width"}},
        {Message::ObjectTypes, {U8("Типы объектов"), "Object types"}},
        {Message::ObjectType, {U8("Тип объекта"), "Object type"}},
        {Message::Tag, {U8("Тег"), "Tag"}},
        {Message::Stats, {U8("Характеристики"), "Stats"}},
        {Message::AddStat, {U8("Добавить"), "Add"}},
        {Message::Levels, {U8("Уровней"), "Levels"}},
        {Message::Level, {U8("Уровень"), "Level"}},
        {Message::LastUpdated, {U8("Обновлено"), "Last updated"}},
        {Message::Rotation, {U8("Поворот (в градусах)"), "Rotation (degrees)"}},
        {Message::Pos, {U8("Расположение"), "Position"}},
        {Message::Pixels, {U8("Пикселей"), "Pixels"}},
        {Message::Type, {U8("Тип"), "Type"}},
        {Message::CantDelete, {U8("Невозможно удалить тип объекта"), "Object type can't be deleted"}},
        {Message::CantDeleteFull,
         {U8("На этом уровне есть %s объектов такого типа. Сначала удалите их, если хотите удалить этот тип"),
          "This level has %s objects of this type, delete them beforehand"}},
        {Message::ActivityStartHint,
         {U8("Координата по 'Y', начиная с которой объект считается активным"),
          "Y-axis coordinate from which object is regarded as active"}},
        {Message::ActivityStart, {U8("Начало активности"), "Activity Start"}},
        {Message::GameFieldHint,
         {U8("Все единицы задаются относительно размера окна, от 0 до 1"),
          "All measures are set relative to window`s size, from 0 to 1"}},
    };

    return messages.at(type)[Lang::current()];
}
