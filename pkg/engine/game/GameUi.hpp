#pragma once

#include "ui/GameOver.hpp"
#include "ui/MainMenu.hpp"
#include "ui/PauseMenu.hpp"
#include "ui/ScoreInput.hpp"
#include "ui/WinScreen.hpp"

namespace engine {
struct GameUi {
    GameOver game_over;
    PauseMenu pause_menu;
    MainMenu main_menu;
    WinScreen win_screen;
    ScoreInput score_input;
};
}  // namespace engine
