#pragma once

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>

#include "Json.hpp"

using KeyList = std::vector<sf::Keyboard::Key>;

template <typename KeyType>
struct Controls {
    KeyType left;
    KeyType right;
    KeyType up;
    KeyType down;

    KeyType shoot;
    KeyType special;
    KeyType slow;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Controls, left, right, up, down, shoot, special, slow)
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

// clang-format off
NLOHMANN_JSON_SERIALIZE_ENUM(sf::Keyboard::Key, {
    {A, "A"},
    {B, "B"},
    {C, "C"},
    {D, "D"},
    {E, "E"},
    {F, "F"},
    {G, "G"},
    {H, "H"},
    {I, "I"},
    {J, "J"},
    {K, "K"},
    {L, "L"},
    {M, "M"},
    {N, "N"},
    {O, "O"},
    {P, "P"},
    {Q, "Q"},
    {R, "R"},
    {S, "S"},
    {T, "T"},
    {U, "U"},
    {V, "V"},
    {W, "W"},
    {X, "X"},
    {Y, "Y"},
    {Z, "Z"},
    {Num0, "Num0"},
    {Num1, "Num1"},
    {Num2, "Num2"},
    {Num3, "Num3"},
    {Num4, "Num4"},
    {Num5, "Num5"},
    {Num6, "Num6"},
    {Num7, "Num7"},
    {Num8, "Num8"},
    {Num9, "Num9"},
    {Escape, "Escape"},
    {LControl, "LControl"},
    {LShift, "LShift"},
    {LAlt, "LAlt"},
    {LSystem, "LSystem"},
    {RControl, "RControl"},
    {RShift, "RShift"},
    {RAlt, "RAlt"},
    {RSystem, "RSystem"},
    {Menu, "Menu"},
    {LBracket, "LBracket"},
    {RBracket, "RBracket"},
    {Semicolon, "Semicolon"},
    {Comma, "Comma"},
    {Period, "Period"},
    {Apostrophe, "Apostrophe"},
    {Slash, "Slash"},
    {Backslash, "Backslash"},
    {Grave, "Grave"},
    {Equal, "Equal"},
    {Hyphen, "Hyphen"},
    {Space, "Space"},
    {Enter, "Enter"},
    {Backspace, "Backspace"},
    {Tab, "Tab"},
    {PageUp, "PageUp"},
    {PageDown, "PageDown"},
    {End, "End"},
    {Home, "Home"},
    {Insert, "Insert"},
    {Delete, "Delete"},
    {Add, "Add"},
    {Subtract, "Subtract"},
    {Multiply, "Multiply"},
    {Divide, "Divide"},
    {Left, "Left"},
    {Right, "Right"},
    {Up, "Up"},
    {Down, "Down"},
    {Numpad0, "Numpad0"},
    {Numpad1, "Numpad1"},
    {Numpad2, "Numpad2"},
    {Numpad3, "Numpad3"},
    {Numpad4, "Numpad4"},
    {Numpad5, "Numpad5"},
    {Numpad6, "Numpad6"},
    {Numpad7, "Numpad7"},
    {Numpad8, "Numpad8"},
    {Numpad9, "Numpad9"},
    {F1, "F1"},
    {F2, "F2"},
    {F3, "F3"},
    {F4, "F4"},
    {F5, "F5"},
    {F6, "F6"},
    {F7, "F7"},
    {F8, "F8"},
    {F9, "F9"},
    {F10, "F10"},
    {F11, "F11"},
    {F12, "F12"},
    {F13, "F13"},
    {F14, "F14"},
    {F15, "F15"},
    {Pause, "Pause"},
})
// clang-format on
