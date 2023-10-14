#pragma once

#include "Controls.hpp"

struct PlayerOptions {
    int num;
    KeyControls keys = kDefaultKeyControls;
    JoyControls joy = kDefaultJoyControls;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(PlayerOptions, keys, joy)

using PlayerList = std::vector<GameObject>;
