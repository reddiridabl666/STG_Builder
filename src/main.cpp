#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{640, 480}), "window");

    bool ok = ImGui::SFML::Init(window);
    if (!ok) {
        std::cout << "Imgui SFML Init failure\n";
        return -1;
    }

    sf::Clock deltaClock;

    while (window.isOpen()) {
        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::Begin("Imgui window");

        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::Text("Hello, world!");
        ImGui::End();

        window.clear(sf::Color::Black);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
