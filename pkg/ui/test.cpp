#include "test.hpp"

#include <imgui.h>

void Test::draw() const {
    ImGui::Begin("Hello, world!");
    ImGui::Text("Hi!!!");
    ImGui::End();
}
