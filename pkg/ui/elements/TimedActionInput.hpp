#pragma once

#include "ActionInput.hpp"

namespace ui {
struct TimedActionInput : public ActionInput {
  public:
    void draw();

  private:
    float timeout = 0;
    float delay = 0;
    bool once = false;

    friend void from_json(const nl::json& json, TimedActionInput& input);
    friend void to_json(nl::json& json, const TimedActionInput& input);
};

struct TimedActionListInput {
  public:
    void draw();

  private:
    std::vector<TimedActionInput> list_;
    long long to_delete_ = -1;

    friend void from_json(const nl::json& json, TimedActionListInput& list);
    friend void to_json(nl::json& json, const TimedActionListInput& list);
};
}  // namespace ui
