#pragma once

struct Event {
    enum Type {
        MouseClick,
        KeyUp,
        KeyDown,
        TypeCount
    };

    Type type;
    int payload;
};
