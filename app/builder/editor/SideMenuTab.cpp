#include "SideMenuTab.hpp"

#include <memory>

#include "Combo.hpp"
#include "ImguiUtils.hpp"
#include "Messages.hpp"
#include "SideMenu.hpp"
#include "ui/common/Bus.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace ui {
namespace {

static const std::vector<std::string> ui_types = {"counter", "max_counter", "bar"};

struct GameUiProps {
    virtual void draw() = 0;
    virtual nl::json to_json() const = 0;
    virtual std::string get_type() const = 0;
    virtual void set_type(const std::string&) = 0;
    virtual std::string get_value() const = 0;
    virtual ~GameUiProps() = default;
};

struct CounterProps : public GameUiProps {
  public:
    virtual void draw() override {
        ImGui::InputText(message(Message::Value), &value);
        ImGui::InputText(message(Message::Prefix), &prefix);
        ImGui::InputText(message(Message::Font), &font);
        ImGui::InputInt(message(Message::Size), &size);
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CounterProps, value, type, prefix, font, size)

    nl::json to_json() const override {
        return *this;
    }

    std::string get_type() const override {
        return type;
    }

    std::string get_value() const override {
        return value;
    }

    void set_type(const std::string& new_type) override {
        type = new_type;
    }

  private:
    std::string value;
    std::string type;
    std::string prefix;
    std::string font;
    int size;
};

struct BarProps : public GameUiProps {
  public:
    virtual void draw() override {
        ImGui::InputText(message(Message::Value), &value);
        ImGui::InputText(message(Message::BarEmpty), &empty);
        ImGui::InputText(message(Message::BarFull), &full);
        ImGui::InputInt(message(Message::Width), &width);
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(BarProps, value, type, empty, full, width)

    nl::json to_json() const override {
        return *this;
    }

    std::string get_type() const override {
        return type;
    }

    std::string get_value() const override {
        return value;
    }

    void set_type(const std::string& new_type) override {
        type = new_type;
    }

  private:
    std::string value;
    std::string type;
    std::string empty;
    std::string full;
    int width = 200;
};

struct GameUiPropsFactory {
    static std::unique_ptr<GameUiProps> create(const nl::json& json);
};

std::unique_ptr<GameUiProps> GameUiPropsFactory::create(const nl::json& json) {
    auto type = json.at("type").get<std::string>();

    if (type == "counter" || type == "max_counter") {
        return json.get<std::unique_ptr<CounterProps>>();
    }

    if (type == "bar") {
        return json.get<std::unique_ptr<BarProps>>();
    }

    return nullptr;
}

struct SideMenuTabContents : public Element {
  public:
    SideMenuTabContents(engine::SideMenuProps&& props, std::vector<std::unique_ptr<GameUiProps>>&& stats)
        : props_(std::move(props)), stats_(std::move(stats)) {}

    void draw(const Window&) override {
        ImGui::BeginGroup();

        ImGui::InputText(message(Message::Background), &props_.bg);

        ImGui::SeparatorText(message(Message::SizeAndOffset));
        ImGui::Text(message(Message::GameFieldHint));

        ImGui::SizeInput(message(Message::Size), &props_.size.width, &props_.size.height);

        ImGui::VecInput(message(Message::Offset), message(Message::LeftEdge), message(Message::TopEdge),
                        &props_.size.left, &props_.size.top);

        ImGui::SeparatorText(message(Message::Layout));

        ImGui::PosInput(message(Message::Offset), &props_.offset.x, &props_.offset.y, "");
        ImGui::NewLine();
        ImGui::InputFloat(message(Message::Gap), &props_.gap);
        ImGui::InputFloat(message(Message::PlayerGap), &props_.player_gap);

        ImGui::SeparatorText(message(Message::Stats));

        bool add = false;

        for (size_t i = 0; i < stats_.size(); ++i) {
            ImGui::BeginGroup();
            auto& elem = stats_[i];

            ImGui::PushID(&elem);

            int id = combo::get_item_id(ui_types, elem->get_type());
            if (ImGui::Combo(message(Message::Type), &id, ui_types)) {
                auto type = combo::find_item(ui_types, id);
                elem = GameUiPropsFactory::create(nl::json{
                    {"type", type},
                    {"value", elem->get_value()},
                });
            }

            elem->draw();

            if (ImGui::Button(message(Message::Delete))) {
                to_delete_ = i;
            }
            ImGui::PopID();

            ImGui::EndGroup();
            if (ImGui::IsItemDeactivatedAfterEdit()) {
                auto json = elem->to_json();
                props_.stats[i] = json;
                json["id"] = i;
                Bus<nl::json>::get().emit(Event::SideMenuElemChanged, json);
            }
            ImGui::NewLine();
        }

        if (ImGui::Button(message(Message::Add))) {
            add = true;
        }
        ImGui::EndGroup();

        if (to_delete_ != -1) {
            stats_.erase(stats_.begin() + to_delete_);
            props_.stats.erase(to_delete_);
            Bus<int>::get().emit(Event::SideMenuElemDeleted, to_delete_);
            to_delete_ = -1;
        }

        if (add) {
            nl::json new_elem = {{"type", "counter"}, {"value", "score"}};
            stats_.push_back(GameUiPropsFactory::create(new_elem));
            props_.stats.push_back(new_elem);
            Bus<nl::json>::get().emit(Event::SideMenuElemAdded, new_elem);
        }

        if (ImGui::IsItemDeactivatedAfterEdit()) {
            Bus<nl::json>::get().emit(Event::SideMenuChanged, props_);
        }
    }

  private:
    engine::SideMenuProps props_;
    std::vector<std::unique_ptr<GameUiProps>> stats_;
    int to_delete_ = -1;
};
}  // namespace

Menu::Tab SideMenuTab(nl::json& json) {
    std::vector<std::unique_ptr<GameUiProps>> stats;
    stats.reserve(json.at("stats").size());

    for (auto& [_, elem] : json.at("stats").items()) {
        auto ui = GameUiPropsFactory::create(elem);
        if (!ui) {
#ifdef DEBUG
            LOG(fmt::format("Error creating GameUI_Props, got: {}", elem.dump(4)));
#endif
            continue;
        }
        stats.push_back(std::move(ui));
    }

    auto tab = std::make_unique<SideMenuTabContents>(json.get<engine::SideMenuProps>(), std::move(stats));
    return Menu::Tab(std::move(tab), message_func(Message::SideMenu));
}
}  // namespace ui
