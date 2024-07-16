#include "menu.hpp"
#include "../hooks/hooks.hpp"
#include "config/config.hpp"
#include "fonts/menu_font.hpp"
#include "fonts/icon.h"
#include "../features/misc/misc.hpp"
#include "../features/movement/movement.hpp"
#include "../features/skins/skins.hpp"
#include "../features/panorama/scaleform.hpp"
#include "../includes/imgui/imgui_internal.h"
#include "../features/skins/skins.hpp"
#include "../features/scripts/scripts.hpp"
#include <string>
#include <map>

bool menu::tab(const char* label, bool selected) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + label_size.x + 5, pos.y + 20));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x + 2.f, rect.Max.y), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);


    window->DrawList->AddLine(ImVec2(rect.Min.x, rect.Min.y + 18), ImVec2(rect.Max.x, rect.Max.y - 2),
        selected ? ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]) :
        ImColor(50 / 255.f, 50 / 255.f, 50 / 255.f, 255.f), 1.f);

    window->DrawList->AddText(ImVec2((rect.Min.x + rect.Max.x) / 2.f - (label_size.x / 2.f), (rect.Min.y + rect.Max.y) / 2.f - (label_size.y / 2.f)),
        selected ? ImColor(255, 255, 255, 255) :
        ImColor(50, 50, 50, 160), label);

    return pressed;
}

bool menu::iskeydown(int key) {
    return HIWORD(GetKeyState(key));
}

bool menu::iskeyup(int key) {
    return !HIWORD(GetKeyState(key));
}

bool menu::checkkey(int key, int keystyle) {
    switch (keystyle) {
    case 0:
        return true;
    case 1:
        return menu::iskeydown(key);
    case 2:
        return LOWORD(GetKeyState(key));
    case 3:
        return menu::iskeyup(key);
    default:
        return true;
    }
}

void  menu::switch_font_cfg(ImFontConfig& f, bool cfg[9]) {
    f.FontBuilderFlags = 0;
    if (cfg[0]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoHinting;
    if (cfg[1]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoAutoHint;
    if (cfg[2]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LightHinting;
    if (cfg[3]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_MonoHinting;
    if (cfg[4]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bold;
    if (cfg[5]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Oblique;
    if (cfg[6]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome;
    if (cfg[7]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
    if (cfg[8]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bitmap;
}

bool menu::toggle_button(const char* label, bool* v, int type, const ImVec2& size_arg) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    int flags = 0;
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos + ImVec2(1, 0 - 2);
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

    if (*v) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImColor(255, 255, 255, int(255)).operator ImVec4());
        ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
        window->DrawList->AddLine(ImVec2(bb.Min.x, bb.Min.y + 18), ImVec2(bb.Max.x, bb.Max.y - 2), ImGui::GetColorU32(ImGuiCol_CheckMark), 0.5);
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Text, ImColor(115, 115, 115, int(255)).operator ImVec4());
        ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
        window->DrawList->AddLine(ImVec2(bb.Min.x, bb.Min.y + 18), ImVec2(bb.Max.x, bb.Max.y - 2), ImColor(27, 27, 27), 0.5);
    }
    ImGui::PopStyleColor();

    if (pressed)
        *v = !*v;

    return pressed;
}
