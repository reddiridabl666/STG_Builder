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
    static inline Value current_lang_ = Value::EN;
};

enum class Message {
    Add,
    ActionType,
    Property,
    Other,
    Count,
    BarEmpty,
    BarFull,
    BulletType,
    Decorators,
    Patterns,
    Repeat,
    RepeatTypes,
    StopAtBounds,
    Time,
    Collisions,
    OnOwnActions,
    OnPlayerActions,
    OnCharacterDeath,
    Games,
    Levels,
    Hitbox,
    Color,
    OutlineColor,
    FillColor,
    Radius,
    Shown,
    Outline,
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
    Background,
    Offset,
    Size,
    Gap,
    PlayerGap,
    LeftEdge,
    TopEdge,
    Height,
    Width,
    Layout,
    SizeAndOffset,
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
    Bomb,
    Value,
    Prefix,
    Font,
};

const char* message(Message type);

inline std::function<const char*()> message_func(Message type) {
    return std::bind(message, type);
}
