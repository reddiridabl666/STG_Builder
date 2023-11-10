#include "Messages.hpp"

#include <array>
#include <unordered_map>

#define U8(str) reinterpret_cast<const char*>(u8##str)

const char* message(Message type) {
    static const std::unordered_map<Message, std::array<const char*, Lang::Count>> messages = {
        {Message::Exit, {U8("Выход"), "Quit"}},
        {Message::Other, {U8("Другое"), "Other"}},
        {Message::StopAtBounds, {U8("Ограничить перемещение границами игрового поля"), "Stop at game field bounds"}},
        {Message::Property, {U8("Характеристика"), "Property"}},
        {Message::Background, {U8("Фон"), "Background"}},
        {Message::Decorators, {U8("Декораторы"), "Decorators"}},
        {Message::Games, {U8("Ваши игры"), "Your games"}},
        {Message::Run, {U8("Запустить"), "Run"}},
        {Message::YourLevels, {U8("Ваши уровни"), "Your levels"}},
        {Message::Delete, {U8("Удалить"), "Delete"}},
        {Message::DeleteLevel, {U8("Удалить уровень"), "Delete level"}},
        {Message::CreateGame, {U8("Создать игру"), "New Game"}},
        {Message::CreateLevel, {U8("Создать уровень"), "New Level"}},
        {Message::CreateObjectType, {U8("Новый тип объекта"), "New object type"}},
        {Message::NewObject, {U8("Создать новый объект"), "New object"}},
        {Message::Back, {U8("Назад"), "Back"}},
        {Message::UnexpectedError, {U8("Непредвиденная ошибка"), "Unexpected error"}},
        {Message::Close, {U8("Закрыть"), "Close"}},
        {Message::Add, {U8("Добавить"), "Add"}},
        {Message::Name, {U8("Название"), "Name"}},
        {Message::Desc, {U8("Описание"), "Description"}},
        {Message::GameField, {U8("Настройки игрового поля"), "Game field options"}},
        {Message::SideMenu, {U8("Боковое меню"), "Side menu options"}},
        {Message::GameOpts, {U8("Игра"), "Game"}},
        {Message::ScreenOpts, {U8("Настройки экрана"), "Screen options"}},
        {Message::WindowSize, {U8("Размер окна"), "Window size"}},
        {Message::Fullscreen, {U8("Полноэкранный режим"), "Fullscreen"}},
        {Message::FPS, {U8("Кадры в секунду"), "Frames per second"}},
        {Message::GameFieldSize, {U8("Отступы и размер игрового поля"), "Game field offset and size"}},
        {Message::Menu, {U8("Меню"), "Menu"}},
        {Message::Layout, {U8("Взаимное расположение эл-тов"), "Layout"}},
        {Message::SizeAndOffset, {U8("Отступы и размер"), "Size and offset"}},
        {Message::LevelOpts, {U8("Уровень"), "Level"}},
        {Message::Speed, {U8("Скорость"), "Speed"}},
        {Message::Hitbox, {U8("Хитбокс"), "Hitbox"}},
        {Message::Color, {U8("Цвет"), "Color"}},
        {Message::OutlineColor, {U8("Цвет границ"), "Outline color"}},
        {Message::FillColor, {U8("Цвет заполнения"), "Fill color"}},
        {Message::Radius, {U8("Радиус"), "Radius"}},
        {Message::Shown, {U8("Показан"), "Shown"}},
        {Message::Outline, {U8("Толщина границ"), "Outline thickness"}},
        {Message::Image, {U8("Изображение"), "Image"}},
        {Message::Offset, {U8("Отступы"), "Offset"}},
        {Message::Size, {U8("Размер"), "Size"}},
        {Message::Gap, {U8("Интервал"), "Gap"}},
        {Message::PlayerGap, {U8("Интервал между блоками хар-к игроков"), "Gap between player's stats"}},
        {Message::LeftEdge, {U8("Слева"), "Left"}},
        {Message::TopEdge, {U8("Сверху"), "Top"}},
        {Message::Height, {U8("Высота"), "Height"}},
        {Message::Width, {U8("Ширина"), "Width"}},
        {Message::ObjectTypes, {U8("Типы объектов"), "Object types"}},
        {Message::ObjectType, {U8("Тип объекта"), "Object type"}},
        {Message::Tag, {U8("Тег"), "Tag"}},
        {Message::Time, {U8("Время"), "Time"}},
        {Message::Stats, {U8("Характеристики"), "Stats"}},
        {Message::AddStat, {U8("Добавить"), "Add"}},
        {Message::Levels, {U8("Уровней"), "Levels"}},
        {Message::Level, {U8("Уровень"), "Level"}},
        {Message::Controls, {U8("Управление"), "Controls"}},
        {Message::Keyboard, {U8("Клавиатура"), "Keyboard"}},
        {Message::Gamepad, {U8("Геймпад"), "Gamepad"}},
        {Message::Left, {U8("Влево"), "Left"}},
        {Message::Right, {U8("Вправо"), "Right"}},
        {Message::Up, {U8("Вверх"), "Up"}},
        {Message::Down, {U8("Вниз"), "Down"}},
        {Message::Collisions, {U8("Коллизии"), "Collisions"}},
        {Message::OnOwnActions, {U8("Обработчики событий объекта"), "On own actions"}},
        {Message::OnPlayerActions, {U8("Обработчики событий игроков"), "On player's actions"}},
        {Message::Rotation, {U8("Поворот (в градусах)"), "Rotation (degrees)"}},
        {Message::Pos, {U8("Расположение"), "Position"}},
        {Message::Pixels, {U8("Пикселей"), "Pixels"}},
        {Message::Type, {U8("Тип"), "Type"}},
        {Message::ActionType, {U8("Тип действия"), "Action type"}},
        {Message::Movement, {U8("Движение"), "Movement"}},
        {Message::Repeat, {U8("Повтор"), "Repeat"}},
        {Message::RepeatTypes, {U8("да\0нет\0последний"), "yes\0no\0last"}},
        {Message::IsAlive, {U8("Жизненный цикл"), "Vital signs"}},
        {Message::Value, {U8("Значение"), "Value"}},
        {Message::Prefix, {U8("Префикс"), "Prefix"}},
        {Message::Font, {U8("Шрифт"), "Font"}},
        {Message::BarEmpty, {U8("Изображение полной шкалы"), "Empty bar image"}},
        {Message::BarFull, {U8("Изображение пустой шкалы"), "Full bar image"}},
        {Message::LastUpdated, {U8("Обновлено: %s"), "Last updated: %s"}},
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
