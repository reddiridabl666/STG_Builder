#include "LevelTab.hpp"

#include "Bus.hpp"
#include "Messages.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace ui {
namespace {
struct FieldOptions {
    float speed = 0;
    std::string image = "";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(FieldOptions, speed, image);

struct LevelTabContents : public Element {
  public:
    LevelTabContents(nl::json& json, const std::function<void()>& cb) : data(json), cb_(cb) {}

    std::string name;
    FieldOptions field;

    void draw(const Window&) override {
        ImGui::InputText(message(Message::Name), &name);

        ImGui::SeparatorText(message(Message::GameField));
        ImGui::InputText(message(Message::Image), &field.image);
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            Bus<std::string>::get().emit(Event::LevelBgChanged, field.image);
        }

        ImGui::InputFloat(message(Message::Speed), &field.speed);

        ImGui::NewLine();

        if (ImGui::Button(message(Message::DeleteLevel))) {
            deleted_ = true;
            cb_();
        }
    }

    ~LevelTabContents() {
        if (deleted_) {
            return;
        }

        try {
            data["name"] = name;
            data["bg"] = field;
            data["last_updated"] = time(nullptr);
        } catch (std::exception& e) {
#ifdef DEBUG
            LOG(e.what());
#endif
        }
    }

  private:
    nl::json& data;
    std::function<void()> cb_;
    bool deleted_ = false;
};
}  // namespace

Menu::Tab LevelTab(nl::json& json, const std::function<void()>& cb) {
    auto tab = std::make_unique<LevelTabContents>(json, cb);

    try {
        tab->name = json.at("name").template get<std::string>();
        tab->field = json.at("bg").template get<FieldOptions>();
    } catch (std::exception& e) {
        fmt::println("{}", e.what());
    }

    return Menu::Tab(std::move(tab), message_func(Message::LevelOpts));
}
}  // namespace ui
