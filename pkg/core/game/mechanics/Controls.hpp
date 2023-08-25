#pragma once

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>

#include "Json.hpp"

using KeyList = std::vector<sf::Keyboard::Key>;

// clang-format off
namespace sf {
NLOHMANN_JSON_SERIALIZE_ENUM(Keyboard::Key, {
    {Keyboard::A, "A"},
    {Keyboard::B, "B"},
    {Keyboard::C, "C"},
    {Keyboard::D, "D"},
    {Keyboard::E, "E"},
    {Keyboard::F, "F"},
    {Keyboard::G, "G"},
    {Keyboard::H, "H"},
    {Keyboard::I, "I"},
    {Keyboard::J, "J"},
    {Keyboard::K, "K"},
    {Keyboard::L, "L"},
    {Keyboard::M, "M"},
    {Keyboard::N, "N"},
    {Keyboard::O, "O"},
    {Keyboard::P, "P"},
    {Keyboard::Q, "Q"},
    {Keyboard::R, "R"},
    {Keyboard::S, "S"},
    {Keyboard::T, "T"},
    {Keyboard::U, "U"},
    {Keyboard::V, "V"},
    {Keyboard::W, "W"},
    {Keyboard::X, "X"},
    {Keyboard::Y, "Y"},
    {Keyboard::Z, "Z"},
    {Keyboard::Num0, "Num0"},
    {Keyboard::Num1, "Num1"},
    {Keyboard::Num2, "Num2"},
    {Keyboard::Num3, "Num3"},
    {Keyboard::Num4, "Num4"},
    {Keyboard::Num5, "Num5"},
    {Keyboard::Num6, "Num6"},
    {Keyboard::Num7, "Num7"},
    {Keyboard::Num8, "Num8"},
    {Keyboard::Num9, "Num9"},
    {Keyboard::Escape, "Escape"},
    {Keyboard::LControl, "LControl"},
    {Keyboard::LShift, "LShift"},
    {Keyboard::LAlt, "LAlt"},
    {Keyboard::LSystem, "LSystem"},
    {Keyboard::RControl, "RControl"},
    {Keyboard::RShift, "RShift"},
    {Keyboard::RAlt, "RAlt"},
    {Keyboard::RSystem, "RSystem"},
    {Keyboard::Menu, "Menu"},
    {Keyboard::LBracket, "LBracket"},
    {Keyboard::RBracket, "RBracket"},
    {Keyboard::Semicolon, "Semicolon"},
    {Keyboard::Comma, "Comma"},
    {Keyboard::Period, "Period"},
    {Keyboard::Apostrophe, "Apostrophe"},
    {Keyboard::Slash, "Slash"},
    {Keyboard::Backslash, "Backslash"},
    {Keyboard::Grave, "Grave"},
    {Keyboard::Equal, "Equal"},
    {Keyboard::Hyphen, "Hyphen"},
    {Keyboard::Space, "Space"},
    {Keyboard::Enter, "Enter"},
    {Keyboard::Backspace, "Backspace"},
    {Keyboard::Tab, "Tab"},
    {Keyboard::PageUp, "PageUp"},
    {Keyboard::PageDown, "PageDown"},
    {Keyboard::End, "End"},
    {Keyboard::Home, "Home"},
    {Keyboard::Insert, "Insert"},
    {Keyboard::Delete, "Delete"},
    {Keyboard::Add, "Add"},
    {Keyboard::Subtract, "Subtract"},
    {Keyboard::Multiply, "Multiply"},
    {Keyboard::Divide, "Divide"},
    {Keyboard::Left, "Left"},
    {Keyboard::Right, "Right"},
    {Keyboard::Up, "Up"},
    {Keyboard::Down, "Down"},
    {Keyboard::Numpad0, "Numpad0"},
    {Keyboard::Numpad1, "Numpad1"},
    {Keyboard::Numpad2, "Numpad2"},
    {Keyboard::Numpad3, "Numpad3"},
    {Keyboard::Numpad4, "Numpad4"},
    {Keyboard::Numpad5, "Numpad5"},
    {Keyboard::Numpad6, "Numpad6"},
    {Keyboard::Numpad7, "Numpad7"},
    {Keyboard::Numpad8, "Numpad8"},
    {Keyboard::Numpad9, "Numpad9"},
    {Keyboard::F1, "F1"},
    {Keyboard::F2, "F2"},
    {Keyboard::F3, "F3"},
    {Keyboard::F4, "F4"},
    {Keyboard::F5, "F5"},
    {Keyboard::F6, "F6"},
    {Keyboard::F7, "F7"},
    {Keyboard::F8, "F8"},
    {Keyboard::F9, "F9"},
    {Keyboard::F10, "F10"},
    {Keyboard::F11, "F11"},
    {Keyboard::F12, "F12"},
    {Keyboard::F13, "F13"},
    {Keyboard::F14, "F14"},
    {Keyboard::F15, "F15"},
    {Keyboard::Pause, "Pause"},
})
}
// clang-format on

template <typename KeyType>
struct Controls {
    KeyType left;
    KeyType right;
    KeyType up;
    KeyType down;

    KeyType shoot;
    KeyType special;
    KeyType slow;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Controls, left, right, up, down, shoot, special, slow)
};

using JoyControls = Controls<unsigned int>;

using KeyControls = Controls<KeyList>;

inline const KeyControls kDefaultKeyControls = {
    .left = {sf::Keyboard::Left, sf::Keyboard::A},
    .right = {sf::Keyboard::Right, sf::Keyboard::D},
    .up = {sf::Keyboard::Up, sf::Keyboard::W},
    .down = {sf::Keyboard::Down, sf::Keyboard::S},
    .shoot = {sf::Keyboard::Space, sf::Keyboard::Z},
    .special = {sf::Keyboard::X, sf::Keyboard::Enter},
    .slow = {sf::Keyboard::LShift, sf::Keyboard::RShift},
};

inline const JoyControls kDefaultJoyControls = {};
