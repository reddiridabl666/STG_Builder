#include "ImguiUtils.hpp"

#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include "Messages.hpp"

static ImVector<ImRect> s_GroupPanelLabelStack;

void ImGui::VecInput(const char* name, const char* x_name, const char* y_name, float* x, float* y) {
    ImGui::BeginGroup();
    ImGui::SeparatorText(name);
    ImGui::InputFloat(x_name, x);
    ImGui::InputFloat(y_name, y);
    ImGui::EndGroup();
}

void ImGui::SizeInput(const char* name, float* x, float* y) {
    ImGui::VecInput(name, message(Message::Width), message(Message::Height), x, y);
}

void ImGui::PosInput(const char* name, float* x, float* y, const std::string& id = "") {
    ImGui::VecInput(name, ("x##" + id).c_str(), ("y##" + id).c_str(), x, y);
}

void ImGui::BeginGroupPanel(const char* name, const ImVec2& size) {
    ImGui::BeginGroup();

    auto itemSpacing = ImGui::GetStyle().ItemSpacing;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

    auto frameHeight = ImGui::GetFrameHeight();
    ImGui::BeginGroup();

    ImVec2 effectiveSize = size;
    if (size.x < 0.0f)
        effectiveSize.x = ImGui::GetContentRegionAvail().x;
    else
        effectiveSize.x = size.x;
    ImGui::Dummy(ImVec2(effectiveSize.x, 0.0f));

    ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
    ImGui::SameLine(0.0f, 0.0f);
    ImGui::BeginGroup();
    ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
    ImGui::SameLine(0.0f, 0.0f);
    ImGui::TextUnformatted(name);
    auto labelMin = ImGui::GetItemRectMin();
    auto labelMax = ImGui::GetItemRectMax();
    ImGui::SameLine(0.0f, 0.0f);
    ImGui::Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));
    ImGui::BeginGroup();

    // ImGui::GetWindowDrawList()->AddRect(labelMin, labelMax, IM_COL32(255, 0, 255, 255));

    ImGui::PopStyleVar(2);

#if IMGUI_VERSION_NUM >= 17301
    ImGui::GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
    ImGui::GetCurrentWindow()->WorkRect.Max.x -= frameHeight * 0.5f;
    ImGui::GetCurrentWindow()->InnerRect.Max.x -= frameHeight * 0.5f;
#else
    ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x -= frameHeight * 0.5f;
#endif
    ImGui::GetCurrentWindow()->Size.x -= frameHeight;

    auto itemWidth = ImGui::CalcItemWidth();
    ImGui::PushItemWidth(ImMax(0.0f, itemWidth - frameHeight));

    s_GroupPanelLabelStack.push_back(ImRect(labelMin, labelMax));
}

void ImGui::EndGroupPanel() {
    ImGui::PopItemWidth();

    auto itemSpacing = ImGui::GetStyle().ItemSpacing;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

    auto frameHeight = ImGui::GetFrameHeight();

    ImGui::EndGroup();

    // ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0,
    // 255, 0, 64), 4.0f);

    ImGui::EndGroup();

    ImGui::SameLine(0.0f, 0.0f);
    ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
    ImGui::Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

    ImGui::EndGroup();

    auto itemMin = ImGui::GetItemRectMin();
    auto itemMax = ImGui::GetItemRectMax();
    // ImGui::GetWindowDrawList()->AddRectFilled(itemMin, itemMax, IM_COL32(255, 0, 0, 64), 4.0f);

    auto labelRect = s_GroupPanelLabelStack.back();
    s_GroupPanelLabelStack.pop_back();

    ImVec2 halfFrame = ImVec2(frameHeight * 0.25f, frameHeight) * 0.5f;
    ImRect frameRect = ImRect(itemMin + halfFrame, itemMax - ImVec2(halfFrame.x, 0.0f));
    labelRect.Min.x -= itemSpacing.x;
    labelRect.Max.x += itemSpacing.x;
    for (int i = 0; i < 4; ++i) {
        switch (i) {
            // left half-plane
            case 0:
                ImGui::PushClipRect(ImVec2(-FLT_MAX, -FLT_MAX), ImVec2(labelRect.Min.x, FLT_MAX), true);
                break;
            // right half-plane
            case 1:
                ImGui::PushClipRect(ImVec2(labelRect.Max.x, -FLT_MAX), ImVec2(FLT_MAX, FLT_MAX), true);
                break;
            // top
            case 2:
                ImGui::PushClipRect(ImVec2(labelRect.Min.x, -FLT_MAX), ImVec2(labelRect.Max.x, labelRect.Min.y), true);
                break;
            // bottom
            case 3:
                ImGui::PushClipRect(ImVec2(labelRect.Min.x, labelRect.Max.y), ImVec2(labelRect.Max.x, FLT_MAX), true);
                break;
        }

        ImGui::GetWindowDrawList()->AddRect(frameRect.Min, frameRect.Max,
                                            ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)), halfFrame.x);

        ImGui::PopClipRect();
    }

    ImGui::PopStyleVar(2);

#if IMGUI_VERSION_NUM >= 17301
    ImGui::GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
    ImGui::GetCurrentWindow()->WorkRect.Max.x += frameHeight * 0.5f;
    ImGui::GetCurrentWindow()->InnerRect.Max.x += frameHeight * 0.5f;
#else
    ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x += frameHeight * 0.5f;
#endif
    ImGui::GetCurrentWindow()->Size.x += frameHeight;

    ImGui::Dummy(ImVec2(0.0f, 0.0f));

    ImGui::EndGroup();
}

bool ImGui::Combo(const char* name, int* cur, const std::vector<std::string>& values) {
    if (BeginCombo(name, values[*cur].c_str())) {
        for (size_t i = 0; i < values.size(); ++i) {
            if (Selectable(values[i].c_str())) {
                *cur = i;
                EndCombo();
                return true;
            }
        }
        EndCombo();
    }
    return false;
}
