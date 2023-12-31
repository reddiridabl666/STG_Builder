#include "App.hpp"

#include <imgui_internal.h>

#include <SFML/System/Clock.hpp>
#include <fstream>

#include "../utils/TimedAction.hpp"
#include "EditorWindow.hpp"
#include "Json.hpp"
#include "Messages.hpp"
#include "ObjectEditor.hpp"
#include "SideMenuTab.hpp"
#include "ui/common/Bus.hpp"
#include "ui/common/Fonts.hpp"
#include "ui/elements/Button.hpp"
#include "ui/elements/ErrorPopup.hpp"
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
static const auto kStatsLocator = "/side_menu/stats"_json_pointer;

App::App(const std::string& games_dir, const std::string& name, uint width, uint height)
    : window_(EditorWindow::init(name, width, height)),
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

    window_.add_handler("app_scroll", sf::Event::MouseWheelScrolled, [this](const sf::Event& event) {
        if (state_.state() != State::LevelEditor || !game_ || ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive()) {
            return;
        }
        game_->scroll(event.mouseWheelScroll.delta * -20);
    });

    window_.add_handler("app_zoom", sf::Event::KeyReleased, [this](const sf::Event& event) {
        if (!game_ || ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive()) {
            return;
        }

        if (event.key.code == sf::Keyboard::Equal) {
            game_->zoom(0.8);
        }
        if (event.key.code == sf::Keyboard::Dash) {
            game_->zoom(1.25);
        }
    });

    window_.add_handler("app_move", sf::Event::KeyPressed, [this, kSpeed = 30](const sf::Event& event) {
        if (state_.state() != State::LevelEditor || !game_ || ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive()) {
            return;
        }

        switch (event.key.code) {
            case sf::Keyboard::Right:
                game_->move_view(kSpeed * sf::Vector2f{1, 0});
                break;
            case sf::Keyboard::Left:
                game_->move_view(kSpeed * sf::Vector2f{-1, 0});
                break;
            case sf::Keyboard::Up:
                game_->move_view(kSpeed * sf::Vector2f{0, -1});
                break;
            case sf::Keyboard::Down:
                game_->move_view(kSpeed * sf::Vector2f{0, 1});
                break;
            default:
                break;
        }
    });

    ui::Bus<std::string>::get().on(ui::Event::GameBgChanged, "app_bg_changed", [this](const std::string& bg) {
        if (game_) {
            game_->set_game_bg(bg);
        }
    });

    ui::Bus<nl::json>::get().on(ui::Event::ObjectTypeChanged, "app_obj_type_changed", [this](const nl::json& json) {
        if (game_) {
            auto name = json.at("name").get<std::string>();
            game_->update_object_type(name, ObjectTypeFactory::generate(name, json));
        }
    });

    ui::Bus<std::string>::get().on(ui::Event::LevelBgChanged, "app_level_bg_changed", [this](const std::string& bg) {
        if (game_) {
            game_->set_level_bg(bg);
        }
    });

    ui::Bus<nl::json>::get().on(ui::Event::SideMenuChanged, "app_side_menu_changed", [this](const nl::json& json) {
        if (game_) {
            builder_.game().at("side_menu") = json;
            game_->update_side_menu(json.get<engine::SideMenuProps>());
        }
    });

    ui::Bus<nl::json>::get().on(ui::Event::SideMenuElemChanged, "app_ui_elem_changed", [this](const nl::json& json) {
        if (game_) {
            auto id = json.at("id").get<size_t>();
            auto tmp = json;
            tmp.erase("id");

            builder_.game().at(kStatsLocator).at(id) = tmp;
            game_->update_menu_item(id, tmp);
        }
    });

    ui::Bus<int>::get().on(ui::Event::SideMenuElemDeleted, "app_ui_elem_deleted", [this](int id) {
        if (game_) {
            game_->erase_menu_item(id);
            builder_.game().at(kStatsLocator).erase(id);
        }
    });

    ui::Bus<nl::json>::get().on(ui::Event::SideMenuElemAdded, "app_ui_elem_added", [this](const nl::json& json) {
        if (game_) {
            game_->add_menu_item(json);
            builder_.game().at(kStatsLocator).push_back(json);
        }
    });
}

App::~App() {
    ui_.clear();
    builder_.save();

    ui::Bus<std::string>::get().off(ui::Event::GameBgChanged, "app_bg_changed");
    ui::Bus<std::string>::get().off(ui::Event::LevelBgChanged, "app_level_bg_changed");
    ui::Bus<nl::json>::get().off(ui::Event::SideMenuChanged, "app_side_menu_changed");
    ui::Bus<int>::get().off(ui::Event::SideMenuElemDeleted, "app_ui_elem_deleted");
    ui::Bus<int>::get().off(ui::Event::SideMenuElemAdded, "app_ui_elem_added");
}

void App::run() noexcept {
    TimedAction saver(
        [this] {
            builder_.save();
        },
        20);

    try {
        saver.run();

        ui::set_default_font(games_dir_ / "Roboto-Regular.ttf", 16);

        window_.main_loop([this, &saver] {
#ifdef DEBUG
            ImGui::ShowDemoWindow();
#endif
            state_.resolve_state_change();
            draw_ui();

            switch (state_.state()) {
                case State::LevelEditor:
                    game_->render_debug();
                    break;
                case State::GamePreview: {
                    auto status = game_->render(game_clock_.restart().asSeconds());
                    if (status == EditableGame::Status::Ended) {
                        state_.schedule_state_change(State::LevelEditor);
                    }
                    break;
                }
                default:
                    break;
            }

            saver.action();
        });
    } catch (const std::exception& e) {
        saver.stop();
        ui::ErrorPopup(window_, e.what());
    } catch (...) {
        saver.stop();
        ui::ErrorPopup(window_, message(Message::UnexpectedError));
    }
}

ui::DefaultBox::Items App::load_games() {
    ui::DefaultBox::Items res;

    res.push_back(std::make_unique<ui::ImageButton>(message_func(Message::CreateGame),
                                                    textures_.get_or("plus.png", assets::kFallbackImage),
                                                    ImVec2{50, 50}, new_game(), true, ImVec2{}, ImVec2{0, 70}));

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

    games_num_ = res.size() - 1;

    return res;
}

ui::DefaultBox::Items App::load_levels() {
    fs::path game_dir = games_dir_ / current_game_;

    ui::DefaultBox::Items res;

    size_t level_num = builder_.game().at("levels").get<size_t>();
    res.reserve(level_num + 1);

    res.push_back(std::make_unique<ui::ImageButton>(
        message_func(Message::CreateLevel), textures_.get_or("plus.png", assets::kFallbackImage), ImVec2{50, 50},
        [this] {
            builder_.new_level();
            state_.schedule_state_change(State::LevelEditor);
        },
        true, ImVec2{}, ImVec2{0, 70}));

    for (size_t num = 1; num <= level_num; ++num) {
        auto filename = fmt::format("level_{}.json", num);

        auto level = json::read(game_dir / filename);

        if (!level) {
            throw std::runtime_error(level.error().message());
        }

        auto bg_image = [&level] -> std::string {
            try {
                return level->at("/bg/image"_json_pointer).template get<std::string>();
            } catch (...) {
                return assets::kFallbackImage;
            }
        }();

        res.push_back(std::make_unique<ui::LevelInfo>(
            level->at("name").template get<std::string>(), num,
            textures_.get_or(game_dir.filename() / kImagesPath / bg_image, assets::kFallbackImage), ImVec2{50, 50},
            [this, num] {
                builder_.choose_level(num - 1);
                state_.schedule_state_change(State::LevelEditor);
            },
            level->value("last_updated", time(nullptr))));
    }

    res.push_back(std::make_unique<ui::ImageButton>(
        message_func(Message::Delete), textures_.get_or("delete.png", assets::kFallbackImage), ImVec2{30, 30},
        [this] {
            builder_.delete_game();
            state_.schedule_state_change(State::Back);
        },
        true, ImVec2{}, ImVec2{0, 48}));

    return res;
}

std::unique_ptr<ui::Element> App::make_menu() {
    std::vector<ui::Menu::Tab> tabs;
    tabs.reserve(4);

    tabs.push_back(ui::MainTab(builder_.game()));

    tabs.push_back(ui::LevelTab(builder_.current_level(), [this] {
        builder_.delete_level();
        state_.schedule_state_change(State::Back);
    }));

    tabs.push_back(ui::SideMenuTab(builder_.game().at("side_menu")));

    tabs.push_back(
        ui::EntitiesTab(current_game_, textures_, builder_.entities(), builder_.current_level().at("entities")));

    return std::make_unique<ui::Menu>(std::move(tabs), message_func(Message::Menu));
}

std::function<void()> App::game_choice(const fs::path& current_game) {
    return [this, current_game] {
        state_.schedule_state_change(State::GameMenu);
        current_game_ = current_game;
    };
}

std::function<void()> App::new_game() {
    return [this] {
        ++games_num_;

        current_game_ = builder_.new_game(games_dir_ / fmt::format("New Game {}", games_num_)).stem();

        state_.schedule_state_change(State::GameMenu);
    };
}

void App::draw_ui() {
    ui::LangChanger::draw(ImVec2{1230, 10}, ImVec2{30, 30});

    for (const auto& [_, elem] : ui_) {
        elem->draw(window_);
    }
}

void App::on_state_start(State state) {
    switch (state) {
        case State::MainMenu: {
            game_.release();
            ui_.erase("back");
            ui_.emplace("exit", std::make_unique<ui::Button>(
                                    message_func(Message::Exit),
                                    [this] {
                                        window_.close();
                                    },
                                    false, ImVec2{1230, 680}));
            ui_.emplace("games", std::make_unique<ui::DefaultBox>(message_func(Message::Games), load_games(),
                                                                  ImVec2{400, 400}, window_.get_center()));
            return;
        }
        case State::GameMenu: {
            auto game_dir = games_dir_ / current_game_;
            builder_.init(game_dir);

            ui_.emplace("back", back_button());
            ui_.emplace("levels", std::make_unique<ui::DefaultBox>(message_func(Message::YourLevels), load_levels(),
                                                                   ImVec2{400, 400}, window_.get_center()));
            game_ = builder_.create_engine(window_);
            return;
        }
        case State::LevelEditor: {
            ui_.emplace("menu", make_menu());
            auto err = game_->choose_level(builder_.current_level_num());
            if (err) {
                throw std::runtime_error(err.message());
            }
            game_->reload_objects();

            ui_.emplace("obj_editor", std::make_unique<ui::ObjectEditor>(window_, *game_, builder_.current_level(),
                                                                         builder_.game(), builder_.entities()));
            ui_.emplace("play_btn", std::make_unique<ui::ImageButton>(
                                        message_func(Message::Run), textures_.get_or("run.png", assets::kFallbackImage),
                                        ImVec2{50, 50},
                                        [this] {
                                            state_.schedule_state_change(State::GamePreview);
                                        },
                                        false, ImVec2{game_->field().right() + 70, game_->field().top() + 30}));
            return;
        }
        case State::GamePreview:
            builder_.save();
            game_ = builder_.create_engine(window_);
            game_->prepare_preview(builder_.current_level_num());
            game_clock_.restart();
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
            ui_.erase("obj_editor");
            ui_.erase("play_btn");
            return;
        case State::GamePreview:
            game_->reset();
            return;
        default:
            return;
    }
}

std::unique_ptr<ui::Element> App::back_button() {
    return std::make_unique<ui::ImageButton>(
        message_func(Message::Back), textures_.get_or("back.png", assets::kFallbackImage), ImVec2{50, 50},
        [this] {
            state_.schedule_state_change(State::Back);
        },
        false, ImVec2{20, 20});
}
}  // namespace builder
