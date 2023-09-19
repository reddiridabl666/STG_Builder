#include "EntitiesTab.hpp"

#include "Box.hpp"
#include "GameInfo.hpp"
#include "ImguiUtils.hpp"
#include "Messages.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

#include <unordered_set>

namespace ui {
namespace {
struct FieldOptions {
    float speed = 0;
    std::string image = "";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(FieldOptions, speed, image);

struct EntityEntry : public Element {
  public:
    EntityEntry(const std::string& name, nl::json& json, std::shared_ptr<sf::Texture>&& image);

    nl::json to_json() const;

    const std::string& get_name() const {
        return name;
    }

    void draw(const Window&) override;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(EntityEntry, image, speed, tag, size)

  private:
    std::shared_ptr<sf::Texture> texture_;

    static const std::unordered_set<std::string> kBaseValues;

    std::string name;
    std::string image;
    std::string tag;
    sf::Vector2f size;
    float speed;

    std::unordered_map<std::string, float> values_;
    bool shown_ = false;
};

struct EntitiesTabContents : public Element {
  public:
    EntitiesTabContents(Box<EntityEntry>::Items&& items, nl::json& data)
        : box_(std::move(items), {}), data(data) {}

    void draw(const Window& window) override {
        box_.draw(window);
    }

    ~EntitiesTabContents() {
        auto backup = data;
        data.clear();
        try {
            for (const auto& entry : box_.elems()) {
                data[entry->get_name()] = entry->to_json();
            }
        } catch (std::exception& e) {
            data = backup;
#ifdef DEBUG
            LOG(e.what());
#endif
        }
    }

  private:
    Box<EntityEntry> box_;
    nl::json& data;
};
}  // namespace

Menu::Tab EntitiesTab(AssetManager<sf::Texture>& textures, nl::json& json) {
    Box<EntityEntry>::Items entries;
    entries.reserve(json.size());

    for (auto& [key, val] : json.items()) {
        entries.push_back(std::make_unique<EntityEntry>(
            key, val, textures.get_or(json::get<std::string>(val, "image"), kFallbackImage)));
    }

    auto tab = std::make_unique<EntitiesTabContents>(std::move(entries), json);

    return Menu::Tab(std::move(tab), message_func(Message::ObjectTypes));
}

const std::unordered_set<std::string> EntityEntry::kBaseValues = {"image", "size", "tag", "speed"};

EntityEntry::EntityEntry(const std::string& name, nl::json& json, std::shared_ptr<sf::Texture>&& image)
    : texture_(std::move(image)), name(name) {
    nl::from_json(json, *this);

    for (auto& [key, val] : json.items()) {
        if (!kBaseValues.contains(key) && val.is_number()) {
            values_[key] = val.template get<float>();
        }
    }
}

nl::json EntityEntry::to_json() const {
    nl::json res;
    nl::to_json(res, *this);

    for (const auto& [key, val] : values_) {
        res[key] = val;
    }

    return res;
}

void EntityEntry::draw(const Window&) {
    ImGui::BeginGroup();
    ImGui::Image(*texture_, sf::Vector2f{50, 50});
    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::Text(name.c_str());
    ImGui::Text("%s: %s", message(Message::Tag), tag.c_str());
    ImGui::EndGroup();

    ImGui::EndGroup();

    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    if (ImGui::IsItemClicked()) {
        shown_ = true;
    }

    if (shown_) {
        ImGui::Begin(name.c_str(), &shown_, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::InputText(message(Message::Name), &name);
        ImGui::InputText(message(Message::Image), &image);
        ImGui::InputText(message(Message::Tag), &tag);
        ImGui::InputFloat(message(Message::Speed), &speed);

        ImGui::SizeInput(message(Message::Size), &size.x, &size.y);

        if (!values_.empty()) {
            ImGui::SeparatorText(message(Message::Stats));
        }

        for (auto& [key, val] : values_) {
            ImGui::InputFloat(key.c_str(), &val);
        }

        ImGui::Button(message(Message::AddStat));

        ImGui::End();
    }
}
}  // namespace ui
