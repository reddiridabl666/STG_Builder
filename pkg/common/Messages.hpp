#pragma once

#include <array>
#include <functional>
#include <string>
#include <unordered_map>

class Lang {
  public:
    enum Value {
        RU,
        EN,
        Count
    };

    static void set(Value lang) {
        current_lang_ = lang;
    }

    static Lang::Value current() {
        return current_lang_;
    }

  private:
    static inline Value current_lang_ = Value::RU;
};

enum class Message {
    Add,
    Repeat,
    RepeatTypes,
    Time,
    Games,
    Levels,
    Run,
    YourLevels,
    Level,
    Exit,
    Delete,
    DeleteLevel,
    Tag,
    CreateGame,
    CreateLevel,
    CreateObjectType,
    NewObject,
    Back,
    UnexpectedError,
    Close,
    AddStat,
    Name,
    Desc,
    GameField,
    GameFieldSize,
    SideMenu,
    GameOpts,
    ScreenOpts,
    FPS,
    WindowSize,
    Fullscreen,
    Menu,
    LevelOpts,
    Speed,
    Image,
    Offset,
    Size,
    LeftEdge,
    TopEdge,
    Height,
    Width,
    GameFieldHint,
    ObjectTypes,
    Stats,
    LastUpdated,
    ObjectType,
    Rotation,
    Pos,
    ActivityStart,
    ActivityStartHint,
    Pixels,
    Type,
    CantDelete,
    CantDeleteFull,
    Movement,
    IsAlive,
    Controls,
    Keyboard,
    Gamepad,
    Left,
    Right,
    Up,
    Down,
    Shoot,
    Slow,
    Bomb
};

const char* message(Message type, Lang::Value lang = Lang::current());

inline std::function<const char*()> message_func(Message type) {
    return std::bind(message, type, Lang::current());
}
