#include "App.hpp"

#include <imgui_internal.h>

#include <SFML/System/Clock.hpp>
#include <fstream>

#include "Json.hpp"
#include "Messages.hpp"
#include "TimedAction.hpp"
#include "ui/elements/Button.hpp"
#include "ui/elements/GameInfo.hpp"
#include "ui/elements/LangChanger.hpp"
#include "ui/elements/LevelInfo.hpp"
#include "ui/elements/Menu.hpp"
#include "ui/elements/Tabs.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace builder {

static constexpr const char* kImagesPath = "assets/images";

App::App(const std::string& games_dir, const std::string& name, uint width, uint height)
    : window_(name, width, height),
      state_(
          [this](State state) {
              return on_state_start(state);
          },
          [this](State state) {
              return on_state_end(state);
          }),
      games_dir_(games_dir),
      textures_(games_dir) {
    Lang::set(Lang::EN);
    state_.schedule_state_change(State::MainMenu);
}

ui::Box::Items App::load_games() {
    ui::Box::Items res;

    res.push_back(std::make_unique<ui::ImageButton>(
        message_func(Message::CreateGame), textures_.get_or("plus.png", kFallbackImage), ImVec2{50, 50},
        new_game(), true, ImVec2{}, ImVec2{0, 70}));

    if (!fs::is_directory(games_dir_)) {
        return res;
    }

    res.reserve(4);

    for (const auto& dir : fs::directory_iterator(games_dir_)) {
        if (!dir.is_directory()) {
            continue;
        }

        auto game = json::read(dir.path() / "game.json");
        if (!game) {
            continue;
        }
#ifdef DEBUG
        LOG(fmt::format("Found game: {}", game->at("name").template get<std::string>()));
#endif
        auto game_info = std::make_unique<ui::GameInfo>(game->template get<ui::GameInfo>());
        game_info->set_cb(game_choice(dir.path().stem()));
        res.push_back(std::move(game_info));
    }

#ifdef DEBUG
    for (size_t i = 0; i < 2; ++i) {
        res.push_back(std::make_unique<ui::GameInfo>("Another game", "Just for UI testing"));
    }
#endif

    games_num_ = res.size() - 1;

    return res;
}

ui::Box::Items App::load_levels() {
    fs::path game_dir = games_dir_ / current_game_;

    ui::Box::Items res;
    res.reserve(4);

    res.push_back(std::make_unique<ui::ImageButton>(
        message_func(Message::CreateLevel), textures_.get_or("plus.png", kFallbackImage), ImVec2{50, 50},
        [this] {
            builder_.new_level();
            state_.schedule_state_change(State::LevelEditor);
        },
        true, ImVec2{}, ImVec2{0, 70}));

    size_t num_pos = sizeof("level");

    for (const auto& file : fs::directory_iterator(game_dir)) {
        auto filename = file.path().filename().string();
        if (!filename.starts_with("level") || !(file.path().extension() == ".json")) {
            continue;
        }

        size_t num = std::stoi(filename.substr(num_pos, filename.find(".") - num_pos));
        auto level = json::read(game_dir / filename);

        if (!level) {
            throw std::runtime_error(level.error().message());
        }

        auto bg_image = [&level] -> std::string {
            try {
                return level->at("/bg/image"_json_pointer).template get<std::string>();
            } catch (...) {
                return kFallbackImage;
            }
        }();

        res.push_back(std::make_unique<ui::LevelInfo>(
            level->at("name").template get<std::string>(), num,
            textures_.get_or(game_dir.filename() / kImagesPath / bg_image, kFallbackImage), ImVec2{50, 50},
            [this, num] {
                builder_.choose_level(num);
                state_.schedule_state_change(State::LevelEditor);
            }));
    }

    return res;
}

std::unique_ptr<ui::Element> App::make_menu() {
    std::vector<ui::Menu::Tab> tabs;
    tabs.reserve(1);
    tabs.push_back(ui::MainTab(builder_.game()));
    return std::make_unique<ui::Menu>(std::move(tabs), message_func(Message::Menu));
}

std::function<void()> App::game_choice(const fs::path& current_game) {
    return [this, current_game] {
        state_.schedule_state_change(State::GameMenu);
        current_game_ = current_game;
    };
}

namespace {
void create_game_files(const fs::path& game) {
    if (!fs::create_directory(game)) {
        throw std::runtime_error("Game directory creation failure");
    }

    if (!fs::create_directories(game / "assets/images")) {
        throw std::runtime_error("Assets directory creation failure");
    }

    nl::json game_json{
        {"name", game.stem().string()},
        {"description", ui::GameInfo::kDefaultDesc},
    };

    json::create(game / "game.json", game_json);

    json::create(game / "entities.json");
}
}  // namespace

std::function<void()> App::new_game() {
    return [this] {
        ++games_num_;
        current_game_ = fmt::format("New Game {}", games_num_);

        create_game_files(games_dir_ / current_game_);

        state_.schedule_state_change(State::GameMenu);
    };
}

void App::run() noexcept try {
    TimedAction saver(
        [this] {
            builder_.save();
        },
        10);

    main_loop([this, &saver] {
        ImGui::ShowDemoWindow();
        state_.resolve_state_change();
        draw_ui();
        saver.action();
    });
} catch (const std::exception& e) {
    ImGui::ErrorCheckEndFrameRecover(nullptr);
    window_.display();
    builder_.save();

    main_loop([&e, this, pop_up = false] mutable {
        if (!pop_up) {
            ImGui::OpenPopup(message(Message::UnexpectedError));
            pop_up = true;
        }

        if (ImGui::BeginPopupModal(message(Message::UnexpectedError), nullptr,
                                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
            ImGui::Text(e.what());
            ImGui::NewLine();

            if (ImGui::Button(message(Message::Close))) {
                window_.close();
            }

            ImGui::EndPopup();
        }
    });
}

void App::main_loop(const std::function<void()>& cb) {
    while (window_.is_open()) {
        window_.process_events();
        window_.clear();

        window_.update_ui();

        cb();

        window_.display();
    }
}

void App::draw_ui() {
    ui::LangChanger::draw(ImVec2{1230, 10}, ImVec2{30, 30});

    for (const auto& [_, elem] : ui_) {
        elem->draw(window_);
    }
}

void App::on_state_start(State state) {
    switch (state) {
        case State::MainMenu:
            ui_.erase("back");
            ui_.emplace("exit", std::make_unique<ui::Button>(
                                    message_func(Message::Exit),
                                    [this] {
                                        window_.close();
                                    },
                                    false, ImVec2{1230, 680}));
            ui_.emplace("games", std::make_unique<ui::Box>(message_func(Message::Games), load_games(),
                                                           ImVec2{400, 400}, window_.get_center()));
            return;
        case State::GameMenu:
#ifdef DEBUG
            LOG(fmt::format("Game chosen: {}", current_game_.string()));
#endif
            ui_.emplace("back", back_button());
            ui_.emplace("levels", std::make_unique<ui::Box>(message_func(Message::Levels), load_levels(),
                                                            ImVec2{400, 400}, window_.get_center()));
            builder_ = GameBuilder::init(games_dir_ / current_game_);
            return;
        case State::LevelEditor:
            ui_.emplace("menu", make_menu());
            return;
        default:
            return;
    }
}

void App::on_state_end(State state) {
    switch (state) {
        case State::MainMenu:
            ui_.erase("games");
            ui_.erase("exit");
            return;
        case State::GameMenu:
            ui_.erase("levels");
            return;
        case State::LevelEditor:
            ui_.erase("menu");
            builder_.save();
            return;
        default:
            return;
    }
}

std::unique_ptr<ui::Element> App::back_button() {
    return std::make_unique<ui::ImageButton>(
        message_func(Message::Back), textures_.get_or("back.png", kFallbackImage), ImVec2{50, 50},
        [this] {
            state_.schedule_state_change(State::Back);
        },
        false, ImVec2{20, 20});
}
}  // namespace builder
