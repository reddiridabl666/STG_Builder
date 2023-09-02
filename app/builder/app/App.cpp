#include "App.hpp"

#include <imgui_internal.h>

#include <SFML/System/Clock.hpp>
#include <fstream>

#include "Json.hpp"
#include "Messages.hpp"
#include "ui/elements/Button.hpp"
#include "ui/elements/GameInfo.hpp"
#include "ui/elements/LangChanger.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

namespace builder {

App::App(const std::string& games_dir, const std::string& name, uint width, uint height)
    : window_(name, width, height), states_{}, games_dir_(games_dir), textures_(games_dir) {
    schedule_state_change(State::MainMenu);
}

std::vector<std::unique_ptr<ui::Element>> App::load_games(const fs::path& games_dir) {
    Lang::set(Lang::EN);

    std::vector<std::unique_ptr<ui::Element>> res;

    res.push_back(std::make_unique<ui::ImageButton>(std::bind(message, Message::CreateGame),
                                                    *textures_.get("plus_invert.png"), ImVec2{50, 50},
                                                    new_game(), ImVec2{0, 70}));

    if (!fs::is_directory(games_dir)) {
        return res;
    }

    res.reserve(4);

    for (const auto& dir : fs::directory_iterator(games_dir)) {
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
        res.push_back(std::make_unique<ui::GameInfo>(game->template get<ui::GameInfo>()));
    }

#ifdef DEBUG
    for (size_t i = 0; i < 2; ++i) {
        res.push_back(std::make_unique<ui::GameInfo>("Another game", "Just for UI testing"));
    }
#endif

    games_num_ = res.size() - 1;

    return res;
}

std::function<void()> App::game_choice(const fs::path& current_game) {
    return [this, current_game] {
        schedule_state_change(State::GameMenu);
        current_game_ = current_game;
    };
}

namespace {
void create_json_file(const fs::path& path, const nl::json& json = "{}"_json) {
    std::ofstream file(path);
    file << std::setw(4) << json;
}

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

    create_json_file(game / "game.json", game_json);

    create_json_file(game / "entities.json");
}
}  // namespace

std::function<void()> App::new_game() {
    return [this] {
        ++games_num_;
        current_game_ = fmt::format("New Game {}", games_num_);

        create_game_files(games_dir_ / current_game_);

        schedule_state_change(State::GameMenu);
    };
}

void App::run() noexcept try {
    main_loop([this] {
        // ImGui::ShowDemoWindow();
        resolve_state_change();
        draw_ui();
    });
} catch (const std::exception& e) {
    ImGui::ErrorCheckEndFrameRecover(nullptr);
    window_.display();

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
    ui::LangChanger::draw(ImVec2{1200, 660}, ImVec2{30, 30});

    for (const auto& [_, elem] : ui_) {
        elem->draw(window_);
    }
}

void App::schedule_state_change(State state) {
    next_state_ = state;
}

void App::resolve_state_change() {
    if (next_state_ == State::Undefined) {
        return;
    }

    if (next_state_ == State::Back) {
        set_prev_state();
    } else {
        set_state(next_state_);
    }

    next_state_ = State::Undefined;
}

void App::on_state_start(State state) {
    switch (state) {
        case State::MainMenu:
            ui_.erase("back");
            ui_.emplace("games", std::make_unique<ui::Box>(load_games(games_dir_), ImVec2{400, 400},
                                                           window_.get_center()));
            return;
        case State::GameMenu:
            ui_.emplace("back", back_button());
            return;
        case State::LevelEditor:
            return;
        default:
            return;
    }
}

void App::on_state_end(State state) {
    switch (state) {
        case State::MainMenu:
            ui_.erase("games");
            return;
        case State::GameMenu:
            return;
        case State::LevelEditor:
            return;
        default:
            return;
    }
}

std::unique_ptr<ui::Element> App::back_button() {
    auto texture = textures_.get("back.png");
    if (!texture) {
        throw std::runtime_error("Missing image: back.png");
    }

    return std::make_unique<ui::ImageButton>(std::bind(message, Message::Back), std::move(*texture),
                                             ImVec2{50, 50}, [this] {
                                                 schedule_state_change(State::Back);
                                             });
}

void App::set_state(App::State state) {
    if (!states_.empty()) {
        on_state_end(states_.top());
    }

    on_state_start(state);
    states_.push(state);
}

void App::set_prev_state() {
    if (states_.size() <= 1) {
        return;
    }

    on_state_end(states_.top());
    states_.pop();
    on_state_start(states_.top());
}

App::State App::state() const {
    return states_.top();
}
}  // namespace builder
