#include "menu.hpp"
#include "config/config.hpp"
#include "fonts/menu_font.hpp"
#include "fonts/icon.h"
#include "../hooks/hooks.hpp"
#include "../features/scripts/scripts.hpp"
#include "../features/misc/misc.hpp"
#include "../features/movement/movement.hpp"
#include "../features/skins/skins.hpp"
#include "../features/panorama/scaleform.hpp"
#include "../includes/imgui/imgui_internal.h"

std::array<float, 3> convert_color(int hex_value) {
    std::array<float, 3> rgb_color;
    rgb_color[2] = ((hex_value >> 16) & 0xFF) / 255.0;
    rgb_color[1] = ((hex_value >> 8) & 0xFF) / 255.0;
    rgb_color[0] = ((hex_value) & 0xFF) / 255.0;
    return rgb_color;
}

template<size_t N>
void render_sub_tabs(const char* (&names)[N], int& activetab, float w, float h) {
    ImGui::PushFont(fonts::menu_font_bold);
    bool values[N] = { false };
    values[activetab] = true;
    for (auto i = 0; i < N; ++i) {
        if (menu::toggle_button(names[i], &values[i], 6, ImVec2{ w, h }))
            activetab = i;
        ImGui::SameLine();
    }
    ImGui::Spacing();
    ImGui::PopFont();
}

ImGuiColorEditFlags no_alpha = ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar;
ImGuiColorEditFlags w_alpha = ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar;

void aimbot_tab() {
    ImGui::PushFont(fonts::menu_font_bold);
    //left upper
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(15);
    ImGui::MenuChild("general", ImVec2(300, 270), false); {
        ImGui::Spacing();
        ImGui::PushFont(fonts::menu_font_thin);
        ImGui::Checkbox("enable aimbot", &c::aimbot::aimbot);
        ImGui::Keybind(("##aimbot key"), &c::aimbot::aimbot_key, &c::aimbot::aimbot_key_s);
        ImGui::Checkbox("silent aim", &c::aimbot::aimbot_silent);
        ImGui::Checkbox("nonsticky", &c::aimbot::non_sticky_aimbot);
        ImGui::Checkbox("backtrack##bt", &c::backtrack::backtrack);
        if (c::backtrack::backtrack) {
            ImGui::Checkbox("aim at backtrack", &c::aimbot::aim_at_bt);
            ImGui::Checkbox("extended", &c::backtrack::fake_latency);
        }
        if (c::backtrack::fake_latency && c::backtrack::backtrack) {
            ImGui::SliderFloat(("##ping"), &c::backtrack::fake, 0, 200, "%.0fms");
        }
        //
        ImGui::PopFont();
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    //left bottom
    ImGui::SetCursorPosY(305);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(15);
    ImGui::MenuChild("triggerbot", ImVec2(300, 270), false); {
        ImGui::PushFont(fonts::menu_font_thin);
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
        ImGui::Spacing();

        ImGui::Checkbox("enable triggerbot", &c::triggerbot::enabled);
        ImGui::Keybind(("##triggerbot key"), &c::triggerbot::triggerbot_key, &c::triggerbot::triggerbot_key_s);

        ImGui::PopFont();
        ImGui::Spacing();
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    //right
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(320);
    ImGui::MenuChild("settings", ImVec2(285, 540), false); {
        ImGui::Spacing();
        ImGui::PushFont(fonts::menu_font_thin);
        static const char* hitboxes[] = { "head","neck","chest","pelvis" };

        ImGui::Text(("weapon"));
        ImGui::Combo("##wwpn", &menu::weapon_selection, "pistol\0heavy pistol\0shotgun\0heavy\0smg\0rifle\0sniper\0auto sniper");
        switch (menu::weapon_selection) {
        case 0:
            ImGui::Text(("hitboxes"));
            ImGui::Combo("##hitbox", &c::aimbot::pistol_hitbox, "head\0neck\0chest\0pelvis");
            ImGui::Text(("field of view"));
            ImGui::SliderInt(("##aimfov1"), &c::aimbot::pistol_aimbot_fov, 0, 180);
            ImGui::Text(("autowall min damage"));
            ImGui::SliderInt(("##awmin1"), &c::aimbot::pistol_autowall_dmg, 1, 100);
            break;
        case 1:
            ImGui::Text(("hitboxes"));
            ImGui::Combo("##hitbox", &c::aimbot::heavy_pistol_hitbox, "head\0neck\0chest\0pelvis");
            ImGui::Text(("field of view"));
            ImGui::SliderInt(("##aimfov2"), &c::aimbot::heavy_pistol_aimbot_fov, 0, 180);
            ImGui::Text(("autowall min damage"));
            ImGui::SliderInt(("##awmin2"), &c::aimbot::heavy_pistol_autowall_dmg, 1, 100);
            break;
        case 2:
            ImGui::Text(("hitboxes"));
            ImGui::Combo("##hitbox", &c::aimbot::shotgun_hitbox, "head\0neck\0chest\0pelvis");
            ImGui::Text(("field of view"));
            ImGui::SliderInt(("##aimfov3"), &c::aimbot::shotgun_aimbot_fov, 0, 180);
            ImGui::Text(("autowall min damage"));
            ImGui::SliderInt(("##awmin3"), &c::aimbot::shotgun_autowall_dmg, 1, 100);
            break;
        case 3:
            ImGui::Text(("hitboxes"));
            ImGui::Combo("##hitbox", &c::aimbot::heavy_hitbox, "head\0neck\0chest\0pelvis");
            ImGui::Text(("field of view"));
            ImGui::SliderInt(("##aimfov4"), &c::aimbot::heavy_aimbot_fov, 0, 180);
            ImGui::Text(("autowall min damage"));
            ImGui::SliderInt(("##awmin4"), &c::aimbot::heavy_autowall_dmg, 1, 100);
            break;
        case 4:
            ImGui::Text(("hitboxes"));
            ImGui::Combo("##hitbox", &c::aimbot::smg_hitbox, "head\0neck\0chest\0pelvis");
            ImGui::Text(("field of view"));
            ImGui::SliderInt(("##aimfov5"), &c::aimbot::smg_aimbot_fov, 0, 180);
            ImGui::Text(("autowall min damage"));
            ImGui::SliderInt(("##awmin5"), &c::aimbot::smg_autowall_dmg, 1, 100);
            break;
        case 5:
            ImGui::Text(("hitboxes"));
            ImGui::Combo("##hitbox", &c::aimbot::rifle_hitbox, "head\0neck\0chest\0pelvis");
            ImGui::Text(("field of view"));
            ImGui::SliderInt(("##aimfov6"), &c::aimbot::rifle_aimbot_fov, 0, 180);
            ImGui::Text(("autowall min damage"));
            ImGui::SliderInt(("##awmin6"), &c::aimbot::rifle_autowall_dmg, 1, 100);
            break;
        case 6:
            ImGui::Text(("hitboxes"));
            ImGui::Combo("##hitbox", &c::aimbot::sniper_hitbox, "head\0neck\0chest\0pelvis");
            ImGui::Text(("field of view"));
            ImGui::SliderInt(("##aimfov7"), &c::aimbot::sniper_aimbot_fov, 0, 180);
            ImGui::Text(("autowall min damage"));
            ImGui::SliderInt(("##awmin7"), &c::aimbot::sniper_autowall_dmg, 1, 100);
            break;
        case 7:
            ImGui::Text(("hitboxes"));
            ImGui::Combo("##hitbox", &c::aimbot::autosniper_hitbox, "head\0neck\0chest\0pelvis");
            ImGui::Text(("field of view"));
            ImGui::SliderInt(("##aimfov8"), &c::aimbot::autosniper_aimbot_fov, 0, 180);
            ImGui::Text(("autowall min damage"));
            ImGui::SliderInt(("##awmin8"), &c::aimbot::autosniper_autowall_dmg, 1, 100);
            break;
        }

        ImGui::PopFont();
        ImGui::Spacing();

    }
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::PopFont();
}

void preview_window() {
    ImGui::PushFont(fonts::menu_font_bold);

    ImGui::SetNextWindowSize(ImVec2(600, 20));
    ImGui::Begin("preview window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar); {

      
    }
    ImGui::End();
}

void visuals_tab() {
    ImGui::PushFont(fonts::menu_font_bold);
    //left upper
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(15);
    ImGui::MenuChild("esp", ImVec2(300, 270), false); {
        ImGui::PushFont(fonts::menu_font_thin);
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
        static const char* flags[6] = { "bot", "armor", "money", "scoped", "flashed", "defusing" };
        static const char* weapon_type[2] = { "text", "icon" };
        static const char* outline_type[2] = { "outter", "inner" };
        static const char* tabs[] = { "enemy","misc","backtrack" };
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        render_sub_tabs(tabs, menu::esp_tab, 299 / 3, 20);
        ImGui::PopStyleVar();
        ImGui::Spacing();

        switch (menu::esp_tab) {
        case 0:
            ImGui::Checkbox(("enable visuals"), &c::visuals::enable_visuals);
            if (c::visuals::enable_visuals) {
                ImGui::Checkbox(("fade animation"), &c::visuals::fade_animation);
                ImGui::Checkbox(("player box"), &c::visuals::playerbox);
                ImGui::SameLine(group_w - 20);
                ImGui::ColorEdit4(("##player box color"), c::visuals::playerbox_color, no_alpha);
                if (c::visuals::playerbox) {
                    ImGui::Text("box type");
                    ImGui::Combo(("##boxtype"), &c::visuals::boxtype, "filled\0corner");
                    ImGui::Text("outline");
                    ImGui::MultiCombo(("##outlinetype"), outline_type, c::visuals::player_box_outline, 2);
                    if (c::visuals::boxtype == 1) {
                        ImGui::Text("corner lenght");
                        ImGui::SliderFloat("##crnerlenght", &c::visuals::corner_lenght, 1.55f, 20.0f, " %.2f");

                    }

                }
                ImGui::Checkbox(("player name"), &c::visuals::playername);
                ImGui::SameLine(group_w - 20);
                ImGui::ColorEdit4(("##name color"), c::visuals::playername_color, no_alpha);
                ImGui::Checkbox(("player hp"), &c::visuals::healthesp);
                ImGui::SameLine(group_w - 20);
                ImGui::ColorEdit4(("##hp color"), c::visuals::health_color, no_alpha);
                if (c::visuals::healthesp) {
                    ImGui::Combo(("##hpesp"), &c::visuals::healthesp_s, "bar\0under player\0only text");
                }
                if (c::visuals::healthesp_s == 0) {
                    //ImGui::Text(("width"));
                    //ImGui::SliderInt(("##width"), &c::visuals::health_bar_width, 1, 10);
                    ImGui::Checkbox(("override color"), &c::visuals::override_bar);
                    ImGui::SameLine(group_w - 20);
                    ImGui::ColorEdit4(("##bar color"), c::visuals::health_bar, no_alpha);
                    if (c::visuals::override_bar) {

                        ImGui::Checkbox(("gradient"), &c::visuals::gradient_bar);
                        ImGui::SameLine(group_w - 20);
                        ImGui::ColorEdit4(("##bar gradient color"), c::visuals::health_bar_gradient, no_alpha);
                    }
                }
                ImGui::Checkbox(("player armor"), &c::visuals::playerarmor);
                ImGui::SameLine(group_w - 20);
                ImGui::ColorEdit4(("##armor color"), c::visuals::playerarmor_clr, no_alpha);

                ImGui::Checkbox(("player weapon"), &c::visuals::playerweapon);
                ImGui::SameLine(group_w - 20);
                ImGui::ColorEdit4(("##player wpn color"), c::visuals::player_weapon_color, no_alpha);
                if (c::visuals::playerweapon) {
                    ImGui::MultiCombo(("##wpntype"), weapon_type, c::visuals::player_weapon_type, 2);
                }

                ImGui::Text(("flags"));
                ImGui::MultiCombo(("##flags"), flags, c::visuals::visuals_flags, 6);
                ImGui::Checkbox(("skeleton"), &c::visuals::skeleton);
                ImGui::SameLine(group_w - 20);
                ImGui::ColorEdit4(("##skeleton clr"), c::visuals::skeletonesp_clr, no_alpha);
            }
            break;
        case 1:
            ImGui::Checkbox(("dropped weapon name"), &c::visuals::dropped_weapon_name);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##dropped weapon name"), c::visuals::dropped_weapon_name_clr, w_alpha);
            ImGui::Checkbox(("dropped weapon icon"), &c::visuals::dropped_weapon_icon);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##dropped weapon iconclr"), c::visuals::dropped_weapon_icon_clr, w_alpha);
            ImGui::Text("max distance");
            ImGui::SliderFloat("##max distance", &c::visuals::dropped_weapon_distance, 150.f, 2000.f, " %.1f");

            ImGui::Checkbox(("out of fov arrows"), &c::visuals::oof_arrows);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##oofarrow1"), c::visuals::oof_arrows_clr, w_alpha);
            ImGui::SameLine(group_w - 40);
            ImGui::ColorEdit4(("##oofarrow2"), c::visuals::oof_arrows_clr2, w_alpha);
            if (c::visuals::oof_arrows) {
                ImGui::Text(("arrow distance"));
                ImGui::SliderInt(("##arrow distance"), &c::visuals::oof_arrows_dist, 0, 100);
                ImGui::Text(("arrow size"));
                ImGui::SliderInt(("##arrow size"), &c::visuals::oof_arrows_size, 0, 30);
            }
            break;
        case 2:
            ImGui::Checkbox(("eye trail"), &c::visuals::skeletonesp);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##bt trail clr"), c::visuals::skeletonbt_clr, no_alpha);
            if (c::visuals::skeletonesp) {
                ImGui::Checkbox(("selected tick"), &c::backtrack::selected_tick);
                ImGui::SameLine(group_w - 20);
                ImGui::ColorEdit4(("##selectedtick"), c::visuals::selected_tick_clr, no_alpha);
            }
            ImGui::Checkbox(("skeleton##bt"), &c::visuals::skeleton_last);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("#skeleton last clr"), c::visuals::skeleton_last_clr, no_alpha);
            break;
        }

        ImGui::PopFont();
        ImGui::Spacing();
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    //left bottom
    ImGui::SetCursorPosY(305);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(15);
    ImGui::MenuChild("glow", ImVec2(300, 270), false); {

        ImGui::PushFont(fonts::menu_font_thin);
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
        ImGui::Spacing();
        ImGui::Checkbox(("enable player glow"), &c::visuals::glow);
        ImGui::SameLine(group_w - 20);
        ImGui::ColorEdit4(("##glowivs"), c::visuals::glow_visible_clr, w_alpha);
        ImGui::SameLine(group_w - 40);
        ImGui::ColorEdit4(("##glowinv"), c::visuals::glow_invisible_clr, w_alpha);

        ImGui::Text("glow style");
        ImGui::Combo(("##boxtype"), &c::visuals::glow_style, "default\0rim\0outline\0pulse\0");


        ImGui::Checkbox(("enable weapon glow"), &c::visuals::glow_weapon);
        ImGui::SameLine(group_w - 20);
        ImGui::ColorEdit4(("##glowpn"), c::visuals::glow_weapon_clr, w_alpha);
        ImGui::Checkbox("player world glow", &c::visuals::dlight);
        ImGui::SameLine(group_w - 20);
        ImGui::ColorEdit4(("##dlight clr"), c::visuals::dlight_clr, w_alpha);
        ImGui::PopFont();
        ImGui::Spacing();
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    //right upper
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(320);
    ImGui::MenuChild("chams", ImVec2(285, 270), false); {
        ImGui::PushFont(fonts::menu_font_thin);
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
        static const char* materials[] = { "regular", "flat", "crystal", "pearlescent", "reverse pearlescent", "fog", "damascus", "model" };
        static const char* chams_overlay_types[] = { "glow", "outline", "metallic", "snow" };
        static const char* local_type[] = { "sleeves","arms","weapons" };
        static const char* tabs[] = { "enemy","local","backtrack" };
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        render_sub_tabs(tabs, menu::chams_tab, 284 / 3, 20);
        ImGui::PopStyleVar();
        ImGui::Spacing();

        switch (menu::chams_tab) {
        case 0:
            ImGui::Checkbox(("enable visible chams"), &c::chams::visible_chams);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##visible clr"), c::chams::visible_chams_clr, w_alpha);
            ImGui::Checkbox(("visible wireframe"), &c::chams::visible_wireframe);
            ImGui::Checkbox(("enable invisible chams"), &c::chams::invisible_chams);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##invisible clr"), c::chams::invisible_chams_clr, w_alpha);
            ImGui::Checkbox(("invisible wireframe"), &c::chams::invisible_wireframe);
            ImGui::Text(("material"));
            ImGui::Combo(("##vismat"), &c::chams::cham_type, materials, IM_ARRAYSIZE(materials));
            ImGui::Checkbox(("overlay chams"), &c::chams::visible_chams_ov);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##ovcolor0"), c::chams::visible_chams_clr_ov, w_alpha);
            if (c::chams::visible_chams_ov) {
                ImGui::Checkbox(("overlay wireframe"), &c::chams::visible_wireframe_ov);
                ImGui::Text(("material"));
                ImGui::Combo(("##overlay"), &c::chams::cham_type_overlay, chams_overlay_types, IM_ARRAYSIZE(chams_overlay_types));
            }

            break;
        case 1:
            /*
            ImGui::Checkbox(("sleeve chams"), &c::chams::sleeve_chams);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##sleeveclr"), c::chams::sleeve_chams_clr, w_alpha);
            ImGui::Checkbox(("sleeve wireframe"), &c::chams::sleeve_wireframe);
            ImGui::Text(("material"));
            ImGui::Combo(("##sleeve"), &c::chams::cham_type_sleeve, materials, IM_ARRAYSIZE(materials));
            ImGui::Checkbox(("overlay sleeve"), &c::chams::sleeve_chams_ov);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##ovcolor"), c::chams::sleeve_chams_clr_ov, w_alpha);
            if (c::chams::sleeve_chams_ov) {
                ImGui::Checkbox(("overlay wireframe"), &c::chams::sleeve_wireframe_ov);
                ImGui::Text(("material"));
                ImGui::Combo(("##overlay2"), &c::chams::cham_type_sleeve_ov, materials, IM_ARRAYSIZE(materials));
            }
            ImGui::Checkbox(("arms chams"), &c::chams::arms_chams);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##armsclr"), c::chams::arms_chams_clr, w_alpha);
            ImGui::Checkbox(("arms wireframe"), &c::chams::arms_wireframe);
            ImGui::Text(("material"));
            ImGui::Combo(("##arms"), &c::chams::cham_type_arms, materials, IM_ARRAYSIZE(materials));
            ImGui::Checkbox(("overlay arms"), &c::chams::arms_chams_ov);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##ovcolorar"), c::chams::arms_chams_clr_ov, w_alpha);
            if (c::chams::arms_chams_ov) {
                ImGui::Checkbox(("overlay wireframe"), &c::chams::arms_wireframe_ov);
                ImGui::Text(("material"));
                ImGui::Combo(("##overlay3"), &c::chams::cham_type_arms_ov, materials, IM_ARRAYSIZE(materials));
            }
            ImGui::Checkbox(("weapon chams"), &c::chams::wpn_chams);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##weaponclr"), c::chams::wpn_chams_clr, w_alpha);
            ImGui::Checkbox(("weapon wireframe"), &c::chams::wpn_wireframe);
            ImGui::Text(("material "));
            ImGui::Combo(("##weapon"), &c::chams::cham_type_wpn, materials, IM_ARRAYSIZE(materials));
            ImGui::Checkbox(("overlay weapon"), &c::chams::wpn_chams_ov);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##ovcolorwpn"), c::chams::wpn_chams_clr_ov, w_alpha);
            if (c::chams::wpn_chams_ov) {
                ImGui::Checkbox(("overlay wireframe"), &c::chams::wpn_wireframe_ov);
                ImGui::Text(("material"));
                ImGui::Combo(("##overlay4"), &c::chams::cham_type_wpn_ov, materials, IM_ARRAYSIZE(materials));
            }
            */
            break;
        case 2:
            ImGui::Checkbox(("enable backtrack chams##bt"), &c::chams::backtrack_chams);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##playerbt1"), c::chams::backtrack_chams_clr2, w_alpha);
            ImGui::Checkbox(("enable invisible chams##bt"), &c::chams::backtrack_chams_invisible);
            ImGui::Checkbox(("draw all ticks##bt"), &c::chams::backtrack_chams_draw_all_ticks);
            if (c::chams::backtrack_chams_draw_all_ticks) {
                ImGui::Checkbox(("gradient##bt"), &c::chams::backtrack_chams_gradient);
                ImGui::SameLine(group_w - 20);
                ImGui::ColorEdit4(("##playerbt2"), c::chams::backtrack_chams_clr1, w_alpha);
            }
            ImGui::Text(("material"));
            ImGui::Combo(("##backtrack"), &c::chams::cham_type_bt, materials, IM_ARRAYSIZE(materials));
            break;
        }

        ImGui::PopFont();
        ImGui::Spacing();
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    //right buttom
    ImGui::SetCursorPosY(305);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(320);
    ImGui::MenuChild("other", ImVec2(285, 270), false); {
        static const char* removals[5] = { "r_3dsky", "mat_postprocess", "cl_shadows", "mat_disable_bloom", "panorama_disable_blur" };
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;

        ImGui::PushFont(fonts::menu_font_thin);
        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text(("world"));
        ImGui::PopFont();
        ImGui::Spacing();

        ImGui::Checkbox("aspect ratio", &c::misc::aspect_ratio);
        if (c::misc::aspect_ratio) {
            ImGui::SliderFloat(("##aspect ratio"), &c::misc::aspect_ratio_amount, 1.f, 2.f, ("%.1f"));
        }
        ImGui::Checkbox("full bright", &c::visuals::fullbright);
        ImGui::Checkbox("fog modulation", &c::visuals::fog);
        ImGui::SameLine(group_w - 20);
        ImGui::ColorEdit4(("##fog color"), c::visuals::fog_color, w_alpha);
        if (c::visuals::fog) {
            ImGui::Text(("fog distance"));
            ImGui::SliderInt(("##fog distance"), &c::visuals::fog_distance, 0, 2500);
            ImGui::Text(("fog density"));
            ImGui::SliderInt(("##fog density"), &c::visuals::fog_density, 0, 100);
        }
        ImGui::Text(("skybox changer"));
        ImGui::Combo(("##skybox"), &c::visuals::skybox, "none\0tibet\0baggage\0italy\0aztec\0vertigo\0daylight\0daylight-2\0clouds\0cloulds-2\0gray\0clear\0canals\0cobblestone\0assault\0cloudsdark\0night\0nigh2\0nightflat\0dusty\0rainy");
        
        ImGui::Checkbox(("precipitation"), &c::visuals::enable_weather);
        ImGui::Checkbox(("world modulate"), &c::visuals::world_modulate);
        ImGui::SameLine(group_w - 20);
        ImGui::ColorEdit4(("##wrldmodulate"), c::visuals::world_color, w_alpha);
        ImGui::Checkbox(("gravity ragdoll"), &c::visuals::gravity_ragdoll);
        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text(("local"));
        ImGui::PopFont();
        ImGui::Spacing();

        ImGui::Checkbox(("zoom"), &c::visuals::apply_zoom);
        if (c::visuals::apply_zoom) {
            ImGui::Keybind(("##zoom key"), &c::visuals::apply_zoom_key, &c::visuals::apply_zoom_key_s);
        }
        ImGui::Checkbox("freecam", &c::misc::freecam);
        if (c::misc::freecam) {
            ImGui::Keybind(("freecam key"), &c::misc::freecam_key, &c::misc::freecam_key_s);
        }
        ImGui::Checkbox(("force crosshair"), &c::misc::force_crosshair);
        ImGui::Checkbox(("sniper crosshair"), &c::misc::sniper_crosshair);
        ImGui::Checkbox(("change weapon sway"), &c::misc::swayscale);
        if (c::misc::swayscale) {
            ImGui::SliderFloat(("##sway"), &c::misc::swayscale_value, 0.f, 20.f, ("%.2f"));
        }
        
        ImGui::Checkbox(("fov"), &c::misc::enable_fov);
        if (c::misc::enable_fov) {
            ImGui::SliderFloat(("##fov"), &c::misc::field_of_view, 0.f, 60.f, ("%.1f"));
        }

        ImGui::Checkbox(("view model"), &c::misc::view_model);
        if (c::misc::view_model) {
            ImGui::Text(("x"));
            ImGui::SliderFloat(("#x"), &c::misc::view_model_x, -15, 15);
            ImGui::Spacing();
            ImGui::Text(("y"));
            ImGui::SliderFloat(("#y"), &c::misc::view_model_y, -15, 15);
            ImGui::Spacing();
            ImGui::Text(("z"));
            ImGui::SliderFloat(("#z"), &c::misc::view_model_z, -15, 15);
        }

        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text(("effects"));
        ImGui::PopFont();

        ImGui::Checkbox("motion blur", &c::visuals::mbenabled);
        if (c::visuals::mbenabled) {
            ImGui::Checkbox("forward enabled", &c::visuals::mbforwardEnabled);
            ImGui::Text(("strength"));
            ImGui::SliderFloat("##strength", &c::visuals::mbstrength, 0.0f, 10.0f, " %.2f");
            ImGui::Text(("falling intensity"));
            ImGui::SliderFloat("##falling intesity", &c::visuals::mbfallingIntensity, 0.0f, 8.0f, " %.2f");
            ImGui::Text(("rotation intensity"));
            ImGui::SliderFloat("##rotation intensity", &c::visuals::mbrotationIntensity, 0.0f, 8.0f, " %.2f");
            ImGui::Text(("falling min"));
            ImGui::SliderFloat("##falling min", &c::visuals::mbfallingMin, 0.0f, 50.0f, " %.2f");
            ImGui::Text(("falling max"));
            ImGui::SliderFloat("##falling max", &c::visuals::mbfallingMax, 0.0f, 50.0f, " %.2f");
        }
        ImGui::Checkbox(("jump trail"), &c::visuals::trails);
        ImGui::SameLine(group_w - 20);
        ImGui::ColorEdit4(("##trail1"), c::visuals::trails_clr1, w_alpha);
        ImGui::Checkbox(("grenade trail"), &c::misc::nadepred);

        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text(("removals"));
        ImGui::PopFont();
        ImGui::Spacing();

        ImGui::Checkbox(("smoke"), &c::visuals::nosmoke);
        ImGui::Checkbox(("flash opacity"), &c::visuals::change_flashalpha);
        if (c::visuals::change_flashalpha) {
            ImGui::SliderInt(("##flashop"), &c::visuals::flashalpha, 1, 255);
        }
        ImGui::Text(("fps booster"));
        ImGui::MultiCombo(("##fps"), removals, c::visuals::removals, 5);
        ImGui::PopFont();
        ImGui::Spacing();
    }
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::PopFont();
}

void misc_tab() {
    ImGui::PushFont(fonts::menu_font_bold);
    //left upper
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(15);
    ImGui::MenuChild("movement", ImVec2(300, 270), false); {
        ImGui::PushFont(fonts::menu_font_thin);
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
        static const char* mov_toggle_type[] = { "always on", "on hotkey", "toggle hotkey" };
        static const char* eb_priority[3] = { "crouching", "standing", "dynamic" };
        static const char* ljnulling[4] = { "-w", "-s", "-a", "-d" };
        static const char* ad_key[3] = { "ej", "mj", "lj" };
        static const char* fb_angles[] = { "right", "backwards", "left" };

        ImGui::Spacing();

        ImGui::Checkbox(("auto bunnyhop"), &c::movement::bhop);
        ImGui::Checkbox(("edgejump"), &c::movement::edge_jump);
        if (c::movement::edge_jump) {
            ImGui::Keybind(("edgejump key"), &c::movement::edge_jump_key, &c::movement::edge_jump_key_s);
            ImGui::Checkbox(("edgejump on ladder"), &c::movement::edge_jump_on_ladder);
            ImGui::Checkbox(("longjump bind"), &c::movement::long_jump_on_edge);
            ImGui::Keybind(("#lj key"), &c::movement::long_jump_key, &c::movement::long_jump_key_s);
        }
        ImGui::Checkbox(("minijump"), &c::movement::mini_jump);
        if (c::movement::mini_jump) {
            ImGui::Keybind(("##minijump key"), &c::movement::mini_jump_key, &c::movement::mini_jump_key_s);
            ImGui::Checkbox(("force edgejump"), &c::movement::mini_jump_ej);
            //ImGui::Checkbox(("remain ducked after jump"), &c::movement::mini_jump_remain_crouched);
        }
        ImGui::Checkbox(("adaptive key cancelling"), &c::movement::adaptive_key_cancelling);
        if (c::movement::adaptive_key_cancelling) {
            ImGui::MultiCombo(("##keynulling"), ljnulling, c::movement::lj_null, 4);
            ImGui::Text(("while"));
            ImGui::MultiCombo(("##adkeyfor"), ad_key, c::movement::adaptive_key_for, 3);
        }
        ImGui::Checkbox(("auto jumpbug"), &c::movement::jump_bug);
        if (c::movement::jump_bug) {
            ImGui::Keybind(("##jump bug key"), &c::movement::jump_bug_key, &c::movement::jump_bug_key_s);
        }
        ImGui::Checkbox(("auto edgebug"), &c::movement::edge_bug);
        if (c::movement::edge_bug) {
            ImGui::Keybind(("edgebug key"), &c::movement::edge_bug_key, &c::movement::edge_bug_key_s);
            ImGui::Text(("scan time"));
            ImGui::SliderFloat(("##seconds to predict"), &c::movement::edge_bug_ticks, 0.f, 1.f, ("%.2fs"));
            ImGui::Text(("mouse lock factor"));
            ImGui::SliderFloat(("##mouse lock percentage"), &c::movement::edge_bug_lock_amount, 0.f, 100.f, ("%.1f%%"));
            ImGui::Checkbox(("advanced search"), &c::movement::edge_bug_advanced_search);
            ImGui::Checkbox(("autostrafe to edge"), &c::movement::edge_bug_rage);
        }

        ImGui::Checkbox(("auto ladderbug"), &c::movement::ladder_bug);
        if (c::movement::ladder_bug) {
            ImGui::Keybind(("ladderglide key"), &c::movement::ladder_bug_key, &c::movement::ladder_bug_key_s);
        }

        ImGui::Checkbox(("auto pixelsurf"), &c::movement::pixel_surf);
        if (c::movement::pixel_surf) {
            ImGui::Keybind(("pixelsurf key"), &c::movement::pixel_surf_key, &c::movement::pixel_surf_key_s);
            ImGui::Checkbox(("freelook surf"), &c::movement::freelook_surf);
            //ImGui::Text(("ps ticks"));
            //ImGui::SliderInt(("##ps ticks"), &c::movement::pixel_surf_ticks, 1, 40);
        }

        ImGui::Checkbox(("auto align"), &c::movement::auto_align);

        ImGui::Checkbox(("delay hop"), &c::movement::delay_hop);
        if (c::movement::delay_hop) {
            ImGui::Keybind(("delay hop key"), &c::movement::delay_hop_key, &c::movement::delay_hop_key_s);
            ImGui::Text(("delay hop ticks"));
            ImGui::SliderInt(("##ticks to wait"), &c::movement::dh_tick, 1, 8);
        }

        //ImGui::Checkbox("fake backwards", &c::movement::fakebackwards);
        //if (c::movement::fakebackwards) {
            //ImGui::Keybind(("fake backwards key"), &c::movement::fakebackwardskey, &c::movement::fakebackwardskey_s);
            //ImGui::Combo(("##fake backwards angle"), &c::movement::fakebackwards_angle, "right\0backwards\0left\0");
        //}

        ImGui::Checkbox(("crouchbug"), &c::movement::crouch_bug);
        if (c::movement::crouch_bug) {
            ImGui::Keybind(("##crouch bug key"), &c::movement::crouch_bug_key, &c::movement::crouch_bug_key_s);
        }

        ImGui::Checkbox(("fast duck"), &c::movement::fastduck);
        ImGui::Checkbox(("auto strafe"), &c::movement::auto_strafe);
        if (c::movement::auto_strafe) {
            ImGui::Keybind(("auto strafe key"), &c::movement::auto_strafe_key, &c::movement::auto_strafe_key_s);
        }
        ImGui::Checkbox(("mouse speed limiter"), &c::movement::strafe_optimizer);
        if (c::movement::strafe_optimizer) {
            ImGui::Keybind(("strafe optimizer key"), &c::movement::strafe_optimizer_key, &c::movement::strafe_optimizer_key_s);
            ImGui::Text(("required speed"));
            ImGui::SliderFloat(("##strafe1"), &c::movement::strafe_optimizer_required_speed, 0.f, 15.f, ("%.1f"));
            ImGui::Text(("desired gain"));
            ImGui::SliderFloat(("##strafe3"), &c::movement::strafe_optimizer_desired_gain, 0.f, 15.f, ("%.1f"));
        }
        ImGui::Spacing();

        ImGui::PopFont();
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    //left bottom
    ImGui::SetCursorPosY(305);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(15);
    ImGui::MenuChild("indicators", ImVec2(300, 270), false); {
        ImGui::PushFont(fonts::menu_font_thin);
        static const char* tabs[] = { "indicators","positions" };
        static const char* indicators[8] = { "eb", "jb", "lj", "mj", "ps", "ej", "lb", "as" };
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        render_sub_tabs(tabs, menu::indicator_tab, 299 / 2, 20);
        ImGui::PopStyleVar();
        ImGui::Spacing();

        switch (menu::indicator_tab) {
        case 0:
            ImGui::Checkbox(("velocity indicator"), &c::movement::velocity_indicator);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##positive"), c::movement::velocity_indicator_positive_clr, no_alpha);
            ImGui::SameLine(group_w - 40);
            ImGui::ColorEdit4(("##negative"), c::movement::velocity_indicator_negative_clr, no_alpha);
            ImGui::SameLine(group_w - 60);
            ImGui::ColorEdit4(("##neutral"), c::movement::velocity_indicator_neutral_clr, no_alpha);
            if (c::movement::velocity_indicator) {
                ImGui::Checkbox(("show takeoff##velocity indicator"), &c::movement::velocity_indicator_show_pre);
                ImGui::Checkbox(("disable takeoff on ground##velocity"), &c::movement::velocity_indicator_disable_ong_show_pre);
                ImGui::Checkbox(("velocity color fade"), &c::movement::velocity_indicator_custom_color);
                ImGui::SameLine(group_w - 20);
                ImGui::ColorEdit4(("##static color"), c::movement::velocity_indicator_custom_clr, w_alpha);
                ImGui::SameLine(group_w - 40);
                ImGui::ColorEdit4(("##static color2"), c::movement::velocity_indicator_fade_clr3, w_alpha);
            }
            ImGui::Checkbox(("stamina indicator"), &c::movement::stamina_indicator);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##stamina indicator color"), c::movement::stamina_indicator_clr, w_alpha);
            if (c::movement::stamina_indicator) {
                if (c::movement::stamina_indicator_fade) {
                    ImGui::SameLine(group_w - 40);
                    ImGui::ColorEdit4(("##stamina indicator intr color"), c::movement::stamina_fade_clr, w_alpha);
                }
                ImGui::Checkbox(("show takeoff##stamina indicator"), &c::movement::stamina_indicator_show_pre);
                ImGui::Checkbox(("disable takeoff on ground##stamina"), &c::movement::stamina_indicator_disable_ong_show_pre);
                ImGui::Checkbox(("stamina color fade"), &c::movement::stamina_indicator_fade);
            }

            ImGui::Text(("indicators"));
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##indicator def clr"), c::movement::velocity_indicator_custom_clr2, no_alpha);

            ImGui::MultiCombo(("##indicators"), indicators, c::movement::indicators_show, 8);
            ImGui::Checkbox(("allow fading animation"), &c::movement::indicators_allow_animation);
            if (c::movement::indicators_allow_animation) {
                ImGui::Text(("fading time"));
                ImGui::SliderFloat(("##fadingtime"), &c::movement::indicators_fading_speed, 25.f, 100.f, ("%.0f%"));
            }
            ImGui::Checkbox(("allow detection color"), &c::movement::allow_detection_clr);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##indicator detection clr"), c::movement::indicator_detect_clr, no_alpha);
            if (c::movement::allow_detection_clr) {
                ImGui::Text(("saved tick amount"));
                ImGui::SliderInt(("##saved tick amount"), &c::movement::detection_saved_tick, 1, 15);
                ImGui::Text(("while"));
                ImGui::MultiCombo(("##detectionfor"), indicators, c::movement::detection_clr_for, 7);
            }

            ImGui::Checkbox(("eb print chat"), &c::movement::edge_bug_detection_printf);
            ImGui::Text(("eb sound"));
            ImGui::Combo(("##ebsound"), &c::movement::edge_bug_detection_sound, "none\0money collect\0switch press\0");
            ImGui::Checkbox(("jb print chat"), &c::movement::jump_bug_detection_printf);
            ImGui::Checkbox(("ps print chat"), &c::movement::pixel_surf_detection_printf);
            //ImGui::Checkbox(("lb print chat"), &c::movement::ladder_bug_detection_printf);
            ImGui::Checkbox(("key strokes"), &c::movement::key_strokes);
            ImGui::PushFont(fonts::menu_font_bold);
            ImGui::Text(("graphs"));
            ImGui::PopFont();
            ImGui::Spacing();

            ImGui::Checkbox(("velocity graph"), &c::movement::velocity_graph);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##graphcolor"), c::movement::velocity_graph_color, no_alpha);
            if (c::movement::velocity_graph) {
                ImGui::Checkbox(("show landed velocity"), &c::movement::velocity_graph_show_landed_speed);
                ImGui::Checkbox(("show edgebugs"), &c::movement::velocity_graph_show_edge_bug);
                ImGui::Checkbox(("show jumpbugs"), &c::movement::velocity_graph_show_jump_bug);

            }
            ImGui::Checkbox(("stamina graph"), &c::movement::stamina_graph);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##stamcolrgraph"), c::movement::stamina_graph_color, no_alpha);

            if (c::movement::velocity_graph || c::movement::stamina_graph) {
                ImGui::Text(("graph width"));
                ImGui::SliderFloat(("##graph width"), &c::movement::velocity_linewidth, 0.f, 10.f, ("%.1f"));
                ImGui::Text(("graph height"));
                ImGui::SliderFloat(("##graph height"), &c::movement::velocity_height, 0.f, 10.f, ("%.1f"));
                ImGui::Text(("graph fade"));
                ImGui::Combo(("##fade_style"), &c::movement::graph_fade, "non faded\0fade start and end\0fade by velocity\0fade only end\0fade only start\0");
                if (c::movement::graph_fade == 2) {
                    ImGui::Text(("graph max fade speed"));
                    ImGui::SliderFloat(("##fade speed"), &c::movement::velocity_tofade, 1.f, 286.f, ("%.1f"));
                }
            }

            ImGui::PushFont(fonts::menu_font_bold);
            ImGui::Text(("other"));
            ImGui::PopFont();
            ImGui::Spacing();

            ImGui::Checkbox(("visualize edgebug"), &c::movement::visualize_edge_bug);
            ImGui::SameLine(group_w - 20);
            ImGui::ColorEdit4(("##vb color"), c::movement::visualize_edge_bug_clr, no_alpha);

            break;
        case 1:
            ImGui::Text(("velocity position"));
            ImGui::SliderInt(("##velocity indicator position"), &c::movement::velocity_indicator_position, 25, 1000);
            ImGui::Text(("stamina position"));
            ImGui::SliderInt(("##stamina indicator position"), &c::movement::stamina_indicator_position, 25, 1000);
            ImGui::Text(("indicators position"));
            ImGui::SliderInt(("##indicators position"), &c::movement::indicators_position, 25, 1000);
            ImGui::Text(("indicators gap"));
            ImGui::SliderInt(("##indicators gap"), &c::movement::indicators_gap, 0, 50);
            ImGui::Text(("key stroke position"));
            ImGui::SliderInt(("##keystroke position"), &c::movement::key_strokes_position, 0, 1000);
            ImGui::Text(("graph position"));
            ImGui::SliderFloat(("##graph pos"), &c::movement::graph_xscreenpos, 0.f, 1.f, ("%.3f"));


            break;
        }

        ImGui::PopFont();
        ImGui::Spacing();

    }
    ImGui::EndChild();
    ImGui::EndGroup();

    //right 
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(320);
    ImGui::MenuChild("miscellaneous", ImVec2(285, 540), false); {
        ImGui::PushFont(fonts::menu_font_thin);
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;

        ImGui::Spacing();
        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text(("clan tag"));
        ImGui::PopFont();
        ImGui::Spacing();
        ImGui::Checkbox(("clan tag spammer"), &c::misc::misc_clantag_spammer);
        if (c::misc::misc_clantag_spammer) {
            ImGui::Text("clan tag type");
            ImGui::Combo(("##cltype"), &c::misc::misc_clantag_type, "static\0custom");
            if (c::misc::misc_clantag_type == 1) {
                ImGui::Checkbox(("animated clan tag"), &c::misc::misc_animated_clantag);
                ImGui::Checkbox(("reverse rolling direction"), &c::misc::misc_clantag_rotation);
                if (c::misc::misc_animated_clantag) {
                    ImGui::Text(("clan tag speed"));
                    ImGui::SliderFloat(("##tag speed"), &c::misc::misc_clantag_speed, 0.1, 2.f, ("%.2f"));
                }
                ImGui::Text("custom clan tag");
                ImGui::PushItemWidth(200.f);
                ImGui::InputText("##clantagtext", c::misc::misc_clantag_text, IM_ARRAYSIZE(c::misc::misc_clantag_text));
            }
        }

        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text(("hitmarkers"));
        ImGui::PopFont();
        ImGui::Spacing();

        ImGui::Checkbox(("hit marker"), &c::misc::misc_hitmarker);
        ImGui::Checkbox(("hit sound"), &c::misc::misc_hitmarker_sound);
        if (c::misc::misc_hitmarker_sound) {
            ImGui::Combo(("##hsoundtype"), &c::misc::misc_hitmarker_sound_type, "arena_switch_press_02\0button22\0money_collect_01\0beep07");
        }
        static const char* hitinfo[2] = { "chat", "screen" };

        ImGui::Text(("hit info"));
        ImGui::MultiCombo(("##hitinfo"), hitinfo, c::misc::misc_hit_info, 2);
        ImGui::Checkbox(("kill effect"), &c::misc::misc_hitmarker_screen_effect);

        ImGui::Checkbox(("kill say"), &c::misc::misc_kill_msg);
        if (c::misc::misc_kill_msg) {
            ImGui::PushItemWidth(200.f);
            ImGui::InputText("##killsaytext", c::misc::misc_kill_message, IM_ARRAYSIZE(c::misc::misc_kill_message));
        }

        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text(("lobby"));
        ImGui::PopFont();
        ImGui::Spacing();

        ImGui::Checkbox(("auto accept"), &c::misc::autoaccept);
        ImGui::Checkbox(("reveal ranks"), &c::misc::misc_reveal_ranks);
        ImGui::Checkbox(("unlock inventory"), &c::misc::unlock_inventory);
        ImGui::Checkbox(("vote revealer"), &c::misc::vote_revealer);

        const char* data_center_list_names[] = { "australia", "austria", "brazil", "chile", "dubai", "france", "germany", "hong kong", "india (chennai)", "india (mumbai)", "japan", "luxembourg",
          "netherlands", "peru", "philipines", "poland", "singapore", "south africa", "spain", "sweden", "uk", "usa (atlanta)", "usa (seattle)", "usa (chicago)", "usa (los angeles)", "usa (moses lake)",
          "usa (oklahoma)", "usa (seattle)", "usa (washington dc)" };

        std::string data_center_list[] = { "syd", "vie", "gru", "scl", "dxb", "par", "fra", "hkg",
           "maa", "bom", "tyo", "lux", "ams", "limc", "man", "waw", "sgp", "jnb",
           "mad", "sto", "lhr", "atl", "eat", "ord", "lax", "mwh", "okc", "sea", "iad" };

        ImGui::Checkbox(("force matchmaking region"), &c::misc::custom_region);
        if (c::misc::custom_region) {
            ImGui::Combo(("##regionselect"), &c::misc::custom_region_selection, data_center_list_names, IM_ARRAYSIZE(data_center_list_names));
        }

        static std::string* force_relay_cluster_value = *(std::string**)(find_pattern(("steamnetworkingsockets.dll"), ("B8 ? ? ? ? B9 ? ? ? ? 0F 43")) + 1);
        if (c::misc::custom_region && !interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
            *force_relay_cluster_value = data_center_list[c::misc::custom_region_selection];
        else if (!c::misc::custom_region && !interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
            *force_relay_cluster_value = "";

        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text(("practice"));
        ImGui::PopFont();
        ImGui::Spacing();

        ImGui::Checkbox("jumpstats", &c::misc::jumpstats);
        if (c::misc::jumpstats) {
            ImGui::Checkbox("show fails", &c::misc::jumpstats_show_fail);
        }
        ImGui::Checkbox("practice", &c::misc::practice);
        if (c::misc::practice) {
            ImGui::Text(("save position"));
            ImGui::Keybind(("save pos"), &c::misc::savepos, &c::misc::savepos_s);
            ImGui::Text(("load position"));
            ImGui::Keybind(("load pos"), &c::misc::loadpos, &c::misc::loadpos_s);
            ImGui::Text(("next position"));
            ImGui::Keybind(("next pos"), &c::misc::nextkey, &c::misc::nextkey_s);
            ImGui::Text(("previous position"));
            ImGui::Keybind(("prev pos"), &c::misc::prevkey, &c::misc::prevkey_s);
            ImGui::Text(("undo position"));
            ImGui::Keybind(("undo pos"), &c::misc::undokey, &c::misc::undokey_s);
        }

        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text(("hud"));
        ImGui::PopFont();
        ImGui::Checkbox("scaleform hud", &c::sfui::sfui_on);
        ImGui::Checkbox(("spotify music display"), &c::misc::show_spotify_currently_playing);
        ImGui::Checkbox(("engine radar"), &c::visuals::radar);
        ImGui::Checkbox(("spectator list"), &c::misc::spectators_list);
        ImGui::SameLine(group_w - 20);
        ImGui::ColorEdit4(("##spectators list clr 1"), c::misc::spectators_list_color_1, no_alpha);
        ImGui::SameLine(group_w - 40);
        ImGui::ColorEdit4(("##spectators list clr 2"), c::misc::spectators_list_color_2, no_alpha);
        if (c::misc::spectators_list) {
            ImGui::Checkbox(("show targets"), &c::misc::spectatorlist_show_target);
            //ImGui::Text(("spectator list type"));
            //ImGui::Combo(("##spectype"), &c::misc::spectatorlist_type, "static\0dynamic\0");
        }
        /*
        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text(("calculator"));
        ImGui::PopFont();
        ImGui::Checkbox("pixelsurf calculator", &c::calculator::ps_calcualtor);
        if (c::calculator::ps_calcualtor) {

            ImGui::Text(("wireframe"));
            ImGui::Keybind(("wireframe key"), &c::calculator::toggle_wireframe_key, &c::calculator::toggle_wireframe_key_s);
            ImGui::Text(("clip brushes"));
            ImGui::Keybind(("clip brushes key"), &c::calculator::toggle_clipbrushe_key, &c::calculator::toggle_clipbrushe_key_s);
            ImGui::Text(("set point"));
            ImGui::Keybind(("set point key"), &c::calculator::set_point_key, &c::calculator::set_point_key_s);
            ImGui::Text(("calculate"));
            ImGui::Keybind(("calculate key"), &c::calculator::calculate_point_key, &c::calculator::calculate_point_key_s);

        }
        */
        
        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text(("other"));
        ImGui::PopFont();
        ImGui::Checkbox(("mouse fix"), &c::misc::mouse_fix);
        ImGui::Checkbox(("pixelsurf fix"), &c::movement::pixel_surf_fix);
        //ImGui::Checkbox(("discord status"), &c::misc::discord_rpc);
        ImGui::Checkbox(("custom console"), &c::misc::custom_console);
        ImGui::SameLine(group_w - 20);
        ImGui::ColorEdit4(("##custom console"), c::misc::custom_console_clr, w_alpha);
        ImGui::Checkbox(("door spam"), &c::misc::door_spam);
        if (c::misc::door_spam) {
            ImGui::Keybind(("door spam key"), &c::misc::door_spam_key, &c::misc::door_spam_key_s);
        }
        if (ImGui::Button("unlock hidden cvars  ")) {
            static bool did_unlock = false;
            if (!did_unlock) {
                auto it = **reinterpret_cast<convar***>(reinterpret_cast<std::uintptr_t>(interfaces::console) + 0x34);
                for (auto c = it->next; c != nullptr; c = c->next) {
                    if (c->flags & fcvar_developmentonly)
                        c->flags &= ~fcvar_developmentonly;

                    if (c->flags & fcvar_hidden)
                        c->flags &= ~fcvar_hidden;
                }

                did_unlock = true;
            }
        }
        ImGui::Spacing();

        ImGui::PopFont();
    }
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::PopFont();
}

std::vector<std::string> menu::get_installed_fonts() {
    std::vector<std::string> fonts;
    fonts.push_back("default");
    for (const auto& entry : std::filesystem::directory_iterator("C:\\Windows\\Fonts")) {
        std::string fontName = entry.path().filename().string();
        if (fontName.find(".ttf") != std::string::npos) {
            fonts.push_back(fontName);
        }
    }
    return fonts;
}

void load_i_fonts() {
    fonts::selected_font_index_main_indi = c::fonts::indi_font;
    if (fonts::selected_font_index_main_indi >= 0) {
        if (menu::fonts[fonts::selected_font_index_main_indi] == "default") {
            fonts::font_directory_indicator = "C:/windows/fonts/tahomabd.ttf";
        }
        else {
            fonts::font_directory_indicator = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_main_indi];
        }
        c::fonts::indi_font = fonts::selected_font_index_main_indi;
    }

    fonts::selected_font_index_sub_indi = c::fonts::sub_indi_font;
    if (fonts::selected_font_index_sub_indi >= 0) {
        if (menu::fonts[fonts::selected_font_index_sub_indi] == "default") {
            fonts::font_directory_sub_indicator = "C:/windows/fonts/tahomabd.ttf";
        }
        else {
            fonts::font_directory_sub_indicator = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_sub_indi];
        }
        c::fonts::sub_indi_font = fonts::selected_font_index_sub_indi;
    }

    fonts::selected_font_index_spec = c::fonts::spec_font;
    if (fonts::selected_font_index_spec >= 0) {
        if (menu::fonts[fonts::selected_font_index_spec] == "default") {
            fonts::font_directory_spec = "C:/windows/fonts/tahomabd.ttf";
        }
        else {
            fonts::font_directory_spec = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_spec];
        }
        c::fonts::spec_font = fonts::selected_font_index_spec;
    }

    fonts::selected_font_index_name = c::fonts::esp_name_font;
    if (fonts::selected_font_index_name >= 0) {
        if (menu::fonts[fonts::selected_font_index_name] == "default") {
            fonts::font_directory_name = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_name = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_name];
        }
        c::fonts::esp_name_font = fonts::selected_font_index_name;
    }

    fonts::selected_font_index_wpn = c::fonts::esp_wpn_font;
    if (fonts::selected_font_index_wpn >= 0) {
        if (menu::fonts[fonts::selected_font_index_wpn] == "default") {
            fonts::font_directory_wpn = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_wpn = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_wpn];
        }
        c::fonts::esp_wpn_font = fonts::selected_font_index_wpn;
    }

    fonts::selected_font_index_dropped_wpn = c::fonts::esp_dropped_wpn_font;
    if (fonts::selected_font_index_dropped_wpn >= 0) {
        if (menu::fonts[fonts::selected_font_index_dropped_wpn] == "default") {
            fonts::font_directory_dropped_wpn = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_dropped_wpn = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_dropped_wpn];
        }
        c::fonts::esp_dropped_wpn_font = fonts::selected_font_index_dropped_wpn;
    }

    fonts::selected_font_index_hp = c::fonts::esp_hp_font;
    if (fonts::selected_font_index_hp >= 0) {
        if (menu::fonts[fonts::selected_font_index_hp] == "default") {
            fonts::font_directory_health = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_health = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_hp];
        }
        c::fonts::esp_hp_font = fonts::selected_font_index_hp;
    }

    fonts::selected_font_index_logs = c::fonts::sc_logs_font;
    if (fonts::selected_font_index_logs >= 0) {
        if (menu::fonts[fonts::selected_font_index_logs] == "default") {
            fonts::font_directory_logs = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_logs = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_logs];
        }
        c::fonts::sc_logs_font = fonts::selected_font_index_logs;
    }
}

void fonts_tab() {
    ImGui::PushFont(fonts::menu_font_bold);
    //left upper
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(15);
    ImGui::MenuChild("fonts", ImVec2(300, 540), false); {
        ImGui::PushFont(fonts::menu_font_thin);
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
        const char* font_flags[] = { "no hinting","no autohint","light hinting","mono hinting","bold","italic","no antialiasing","load color","bitmap","dropshadow","outline" };
        const char* fnt_tab[] = { "main indicator font", "sub indicator font", "spec font", "name font", "health font", "player weapon font", "dropped weapon font", "screen logs font" };
        static ImGuiTextFilter filter;

        load_i_fonts();

        ImGui::Text("selected font");
        ImGui::Combo(("##fnt"), &menu::font_tab, fnt_tab, IM_ARRAYSIZE(fnt_tab));

        ImGui::Text(("font list"));
        ImGui::Text((""));
        ImGui::SameLine(group_w - 283);

        if (menu::font_tab == 0) {
            //main indicators
            ImGui::ListBoxHeader("##fntlist_indicators", ImVec2(285, 290)); {
                for (int i = 0; i < menu::fonts.size(); i++) {
                    
                    std::string fonts = menu::fonts[i];

                    if (filter.PassFilter(fonts.c_str())) {
                        if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_main_indi)) {
                            fonts::selected_font_index_main_indi = i;
                        }
                    }
                }
                ImGui::ListBoxFooter();
            }

            if (fonts::selected_font_index_main_indi >= 0) {
                if (menu::fonts[fonts::selected_font_index_main_indi] == "default") {
                    fonts::font_directory_indicator = "C:/windows/fonts/tahomabd.ttf";
                }
                else {
                    fonts::font_directory_indicator = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_main_indi];
                }
                c::fonts::indi_font = fonts::selected_font_index_main_indi;
            }
            
            ImGui::Text(("search font"));
            filter.Draw("##filter_skin");
            ImGui::Text("font size");
            ImGui::SliderInt(("##indi-size"), &c::fonts::indi_size, 1, 50);
            ImGui::Text("font flags");
            ImGui::MultiCombo(("##flags-for-indicators"), font_flags, c::fonts::indi_font_flag, 11);
        }
        else if (menu::font_tab == 1) {
            //sub indicators

            ImGui::ListBoxHeader("##fntlist_indicators", ImVec2(285, 290)); {
                for (int i = 0; i < menu::fonts.size(); i++) {

                    std::string fonts = menu::fonts[i];

                    if (filter.PassFilter(fonts.c_str())) {
                        if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_sub_indi)) {
                            fonts::selected_font_index_sub_indi = i;
                        }
                    }
                }
                ImGui::ListBoxFooter();
            }

            if (fonts::selected_font_index_sub_indi >= 0) {
                if (menu::fonts[fonts::selected_font_index_sub_indi] == "default") {
                    fonts::font_directory_sub_indicator = "C:/windows/fonts/tahomabd.ttf";
                }
                else {
                    fonts::font_directory_sub_indicator = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_sub_indi];
                }
                c::fonts::sub_indi_font = fonts::selected_font_index_sub_indi;
            }
            
            ImGui::Text(("search font"));
            filter.Draw("##filter_skin");
            ImGui::Text("font size");
            ImGui::SliderInt(("##sub-indi-size"), &c::fonts::sub_indi_size, 1, 50);
            ImGui::Text("font flags");
            ImGui::MultiCombo(("##flags-for-sub_indicators"), font_flags, c::fonts::sub_indi_font_flag, 11);
        }
        else if (menu::font_tab == 2) {
            //spec main

            ImGui::ListBoxHeader("##fntlist_spec", ImVec2(285, 290)); {
                for (int i = 0; i < menu::fonts.size(); i++) {

                    std::string fonts = menu::fonts[i];

                    if (filter.PassFilter(fonts.c_str())) {
                        if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_spec)) {
                            fonts::selected_font_index_spec = i;
                        }
                    }
                }
                ImGui::ListBoxFooter();
            }
            
            if (fonts::selected_font_index_spec >= 0) {
                if (menu::fonts[fonts::selected_font_index_spec] == "default") {
                    fonts::font_directory_spec = "C:/windows/fonts/tahomabd.ttf";
                }
                else {
                    fonts::font_directory_spec = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_spec];
                }
                c::fonts::spec_font = fonts::selected_font_index_spec;
            }

            ImGui::Text(("search font"));
            filter.Draw("##filter_font");
            ImGui::Text("font size");
            ImGui::SliderInt(("##sec-size"), &c::fonts::spec_size, 1, 50);
            ImGui::Text("font flags");
            ImGui::MultiCombo(("##flags-for-spec"), font_flags, c::fonts::spec_font_flag, 9);
        }
        else if (menu::font_tab == 3) {
            //name

            ImGui::ListBoxHeader("##fntlist_name", ImVec2(285, 290)); {
                for (int i = 0; i < menu::fonts.size(); i++) {

                    std::string fonts = menu::fonts[i];

                    if (filter.PassFilter(fonts.c_str())) {
                        if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_name)) {
                            fonts::selected_font_index_name = i;
                        }
                    }
                }
                ImGui::ListBoxFooter();
            }

            if (fonts::selected_font_index_name >= 0) {
                if (menu::fonts[fonts::selected_font_index_name] == "default") {
                    fonts::font_directory_name = "C:/windows/fonts/tahoma.ttf";
                }
                else {
                    fonts::font_directory_name = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_name];
                }
                c::fonts::esp_name_font = fonts::selected_font_index_name;
            }

            ImGui::Text(("search font"));
            filter.Draw("##filter_font");
            ImGui::Text("font size");
            ImGui::SliderInt(("##name-size"), &c::fonts::esp_name_size, 1, 50);
            ImGui::Text("font flags");
            ImGui::MultiCombo(("##flags-for-name"), font_flags, c::fonts::esp_name_font_flag, 11);
        }
        else if (menu::font_tab == 4) {
            //health

            ImGui::ListBoxHeader("##fntlist_hp", ImVec2(285, 290)); {
                for (int i = 0; i < menu::fonts.size(); i++) {
                    
                    std::string fonts = menu::fonts[i];

                    if (filter.PassFilter(fonts.c_str())) {
                        if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_hp)) {
                            fonts::selected_font_index_hp = i;
                        }
                    }
                }
                ImGui::ListBoxFooter();
            }
            
            if (fonts::selected_font_index_hp >= 0) {
                if (menu::fonts[fonts::selected_font_index_hp] == "default") {
                    fonts::font_directory_health = "C:/windows/fonts/tahoma.ttf";
                }
                else {
                    fonts::font_directory_health = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_hp];
                }
                c::fonts::esp_hp_font = fonts::selected_font_index_hp;
            }

            ImGui::Text(("search font"));
            filter.Draw("##filter_font");
            ImGui::Text("font size");
            ImGui::SliderInt(("##hp-size"), &c::fonts::esp_hp_size, 1, 50);
            ImGui::Text("font flags");
            ImGui::MultiCombo(("##flags-for-h["), font_flags, c::fonts::esp_font_flag, 11);

        }
        else if (menu::font_tab == 5) {
            //player weapon

            ImGui::ListBoxHeader("##fntlist_wpn", ImVec2(285, 290)); {
                for (int i = 0; i < menu::fonts.size(); i++) {

                    std::string fonts = menu::fonts[i];

                    if (filter.PassFilter(fonts.c_str())) {
                        if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_wpn)) {
                            fonts::selected_font_index_wpn = i;
                        }
                    }
                }
                ImGui::ListBoxFooter();
            }

            if (fonts::selected_font_index_wpn >= 0) {
                if (menu::fonts[fonts::selected_font_index_wpn] == "default") {
                    fonts::font_directory_wpn = "C:/windows/fonts/tahoma.ttf";
                }
                else {
                    fonts::font_directory_wpn = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_wpn];
                }
                c::fonts::esp_wpn_font = fonts::selected_font_index_wpn;
            }
            
            ImGui::Text(("search font"));
            filter.Draw("##filter_font");
            ImGui::Text("font size");
            ImGui::SliderInt(("##wpn_size"), &c::fonts::esp_wpn_size, 1, 50);
            ImGui::Text("font flags");
            ImGui::MultiCombo(("##flags-for-wpn"), font_flags, c::fonts::esp_wpn_font_flag, 11);
        }
        else if (menu::font_tab == 6) {
            //dropped weapon

            ImGui::ListBoxHeader("##fntlist_dropped_wpn", ImVec2(285, 290)); {
                for (int i = 0; i < menu::fonts.size(); i++) {
  
                    std::string fonts = menu::fonts[i];

                    if (filter.PassFilter(fonts.c_str())) {
                        if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_dropped_wpn)) {
                            fonts::selected_font_index_dropped_wpn = i;
                        }
                    }
                }
                ImGui::ListBoxFooter();
            }
            
            if (fonts::selected_font_index_dropped_wpn >= 0) {
                if (menu::fonts[fonts::selected_font_index_dropped_wpn] == "default") {
                    fonts::font_directory_dropped_wpn = "C:/windows/fonts/tahoma.ttf";
                }
                else {
                    fonts::font_directory_dropped_wpn = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_dropped_wpn];
                }
                c::fonts::esp_dropped_wpn_font = fonts::selected_font_index_dropped_wpn;
            }

            ImGui::Text(("search font"));
            filter.Draw("##filter_font");
            ImGui::Text("font size");
            ImGui::SliderInt(("##droppped_size"), &c::fonts::esp_dropped_wpn_size, 1, 50);
            ImGui::Text("font flags");
            ImGui::MultiCombo(("##flags-for-dropped_wpn"), font_flags, c::fonts::esp_dropped_wpn_font_flag, 11);

        }
        else if (menu::font_tab == 7) {
            //dropped weapon

            ImGui::ListBoxHeader("##fntlist_sc_logs", ImVec2(285, 290)); {
                for (int i = 0; i < menu::fonts.size(); i++) {

                    std::string fonts = menu::fonts[i];

                    if (filter.PassFilter(fonts.c_str())) {
                        if (ImGui::Selectable(menu::fonts[i].c_str(), i == fonts::selected_font_index_logs)) {
                            fonts::selected_font_index_logs = i;
                        }
                    }
                }
                ImGui::ListBoxFooter();
            }

            fonts::selected_font_index_logs = c::fonts::sc_logs_font;
            if (fonts::selected_font_index_logs >= 0) {
                if (menu::fonts[fonts::selected_font_index_logs] == "default") {
                    fonts::font_directory_logs = "C:/windows/fonts/tahoma.ttf";
                }
                else {
                    fonts::font_directory_logs = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_logs];
                }
                c::fonts::sc_logs_font = fonts::selected_font_index_logs;
            }

            ImGui::Text(("search font"));
            filter.Draw("##filter_font");
            ImGui::Text("font size");
            ImGui::SliderInt(("##sc-logs_size"), &c::fonts::sc_logs_size, 1, 50);
            ImGui::Text("font flags");
            ImGui::MultiCombo(("##flags-for-sc-logs"), font_flags, c::fonts::sc_logs_flag, 11);
        }


        if (ImGui::Button("reset fonts  "))
            fonts::reset_fonts();

        if (ImGui::Button(("reload fonts  "))) {
            std::stringstream ss;

            ss << "\x08" << " \x08"
                << "%c"
                << "delusional"
                << "\x08" << "\x08 | reloaded fonts ";

            if (interfaces::engine->is_in_game()) {
                interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
            }
            menu::fonts_initialized = true;
            load_i_fonts();
            surface::initialize();
        }

        ImGui::PopFont();
    }
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::PopFont();
}

void skins_tab() {
    ImGui::PushFont(fonts::menu_font_bold);
    //left upper
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(15);
    ImGui::MenuChild("item", ImVec2(300, 270), false); {
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;

        ImGui::Spacing();
        ImGui::PushFont(fonts::menu_font_thin);

        ImGui::Text(("item"));
        ImGui::Combo("##item", &menu::skin_selection, "knife\0glove\0");

        switch (menu::skin_selection) {
        case 0:
            ImGui::Checkbox(("enabled##knife"), &c::skins::knife_changer_enable);
            ImGui::Combo(("##knifemodel"), &c::skins::knife_changer_model, "default\0bayonet\0m9\0karambit\0bowie\0butterfly\0falchion\0flip\0gut\0huntsman\0shaddow-daggers\0navaja\0stiletto\0talon\0ursus\0default ct\0default t\0skeleton\0");
            break;

        case 1:
            ImGui::Checkbox(("enabled##gloves"), &c::skins::gloves_endable);
            ImGui::Combo("##glovesmodel", &c::skins::gloves_model, "default\0blood\0sport\0slick\0leather\0moto\0speci\0hydra");
            break;
        }

        ImGui::PopFont();
        ImGui::Spacing();

    }
    ImGui::EndChild();
    ImGui::EndGroup();

    //left bottom
    ImGui::SetCursorPosY(305);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(15);
    ImGui::MenuChild("other", ImVec2(300, 270), false); {
        ImGui::Spacing();
        ImGui::PushFont(fonts::menu_font_thin);

        ImGui::Checkbox(("model changer##agent changer"), &c::skins::agent_changer);
        if (c::skins::agent_changer) {
            ImGui::Text(("terrorist"));
            ImGui::Combo(("##terrorist"), &c::skins::agent_t, menu::agents, IM_ARRAYSIZE(menu::agents));
            ImGui::Text(("counter terrorist"));
            ImGui::Combo(("##counter terrorist"), &c::skins::agent_ct, menu::agents, IM_ARRAYSIZE(menu::agents));
        }

        ImGui::PopFont();
        ImGui::Spacing();

    }
    ImGui::EndChild();
    ImGui::EndGroup();

    //right 
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(320);
    ImGui::MenuChild("skins", ImVec2(285, 540), false); {
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
        static char search_knife_skin[256] = ""; 
        static char search_glove_skin[256] = "";
        static ImGuiTextFilter filter;

        ImGui::PushFont(fonts::menu_font_thin);
        ImGui::Spacing();

        switch (menu::skin_selection) {
        case 0:
            ImGui::Text(("knife skin"));
            ImGui::Text((""));
            ImGui::SameLine(group_w - 269);
            ImGui::ListBoxHeader("##knifeskin", ImVec2(272, 360)); {
                std::string name = ("elpepe");
                for (int i = 0; i < features::skins::parser_skins.size(); i++) {
                    bool is_selected = (c::skins::knife_changer_vector_paint_kit == i);

                    std::string name = features::skins::parser_skins[i].name;

                    if (filter.PassFilter(name.c_str())) {

                        if (name == features::skins::parser_skins[i].name)
                            ImGui::PushID(std::hash<std::string>{}(features::skins::parser_skins[i].name)* i);

                        if (ImGui::Selectable(features::skins::parser_skins[i].name.c_str(), &is_selected)) {
                            c::skins::knife_changer_vector_paint_kit = i;
                            c::skins::knife_changer_paint_kit = features::skins::parser_skins[c::skins::knife_changer_vector_paint_kit].id;
                        }

                        if (name == features::skins::parser_skins[i].name)
                            ImGui::PopID();

                        name = features::skins::parser_skins[i].name;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::ListBoxFooter();
            }

            ImGui::Text(("search skin"));
            filter.Draw("##filter_skin");

            ImGui::Text(("wear"));
            ImGui::Combo(("##knifecondition"), &c::skins::knife_changer_wear, "factory-new\0minimal-wear\0field-tested\0well-worn\0battle-scarred");

            ImGui::Checkbox(("custom color"), &c::skins::skin_custom_clr);
            if (c::skins::skin_custom_clr) {
                ImGui::SameLine(group_w - 20);
                ImGui::ColorEdit4(("##modulate1"), c::skins::skin_modulation1, no_alpha);
                ImGui::SameLine(group_w - 40);
                ImGui::ColorEdit4(("##modulate2"), c::skins::skin_modulation2, no_alpha);
                ImGui::SameLine(group_w - 60);
                ImGui::ColorEdit4(("##modulate3"), c::skins::skin_modulation3, no_alpha);
                ImGui::SameLine(group_w - 80);
                ImGui::ColorEdit4(("##modulate4"), c::skins::skin_modulation4, no_alpha);                
            }
            break;
        case 1:
            ImGui::Text(("glove skin"));
            ImGui::Text((""));
            ImGui::SameLine(group_w - 269);
            ImGui::ListBoxHeader("##glovesskin", ImVec2(272, 360)); {
                std::string name = ("elpepe");
                for (int i = 0; i < features::skins::parser_gloves.size(); i++) {
                    bool is_selected = (c::skins::gloves_skin == i);

                    std::string name = features::skins::parser_gloves[i].name;

                    if (filter.PassFilter(name.c_str())) {

                        if (name == features::skins::parser_gloves[i].name)
                            ImGui::PushID(std::hash<std::string>{}(features::skins::parser_gloves[i].name)* i);

                        if (ImGui::Selectable(features::skins::parser_gloves[i].name.c_str(), &is_selected)) {
                            c::skins::gloves_skin = i;
                            c::skins::gloves_skin_id = features::skins::parser_gloves[c::skins::gloves_skin].id;
                        }

                        if (name == features::skins::parser_gloves[i].name)
                            ImGui::PopID();

                        name = features::skins::parser_gloves[i].name;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::ListBoxFooter();
            }


            ImGui::BeginListBox("##glovesskin", ImVec2(272, 360)); {
                for (int i = 0; i < features::skins::parser_gloves.size(); i++) {
                    std::string name = features::skins::parser_gloves[i].name;

                    if (filter.PassFilter(name.c_str())) {

                        std::string label = features::skins::parser_gloves[i].name + "##" + std::to_string(i);

                        if (ImGui::Selectable(label.c_str(), c::skins::gloves_skin == i)) {
                            c::skins::gloves_skin = i;
                            c::skins::gloves_skin_id = features::skins::parser_gloves[c::skins::gloves_skin].id;
                        }
                    }
                }
                ImGui::EndListBox();
            }

            ImGui::Text(("search skin"));
            filter.Draw("##filter_skin_gloves");
            ImGui::Text(("wear"));
            ImGui::Combo("##gloveswear", &c::skins::gloves_wear, "factory-new\0minimal-wear\0field-tested\0well-worn\0battle-scarred");

            break;
        }

        if (ImGui::Button(("update  "))) {
            features::misc::notify(("force updated"), color(255, 255, 255, 255));
            std::stringstream ss;

            ss << "\x08" << " \x08" << "%c" << "delusional" << "\x08" << "\x08 | force updated";

            if (interfaces::engine->is_in_game()) {
                interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
                features::skins::forcing_update = true;
            }
        }

        ImGui::PopFont();
        ImGui::Spacing();
    }
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::PopFont();
}

void mr_tab() {
    ImGui::PushFont(fonts::menu_font_bold);
    //left upper
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(15);
    ImGui::MenuChild("movement recorder", ImVec2(300, 540), false); {
        ImGui::PushFont(fonts::menu_font_thin);
        ImGui::Spacing();
        ImGui::Checkbox(("enable recorder##mr"), &c::misc::movement_rec);
        if (c::misc::movement_rec) {

            ImGui::Checkbox(("show recording line"), &c::misc::movement_rec_show_line);
            ImGui::Checkbox(("original playback viewangles"), &c::misc::movement_rec_lockva); //mouse
            ImGui::Text("smoothing");
            ImGui::SliderFloat(("##smoothing"), &c::misc::movement_rec_smoothing, 1.f, 100.f, ("%.3f"));
            ImGui::Spacing();

            ImGui::Text(("start recording"));
            ImGui::Keybind(("start rec"), &c::misc::movement_rec_keystartrecord);

            ImGui::Text(("start playback"));
            ImGui::Keybind(("start playback"), &c::misc::movement_rec_keystartplayback);

            ImGui::Text(("stop recording"));
            ImGui::Keybind(("stop rec"), &c::misc::movement_rec_keystoprecord);

            ImGui::Text(("stop playback"));
            ImGui::Keybind(("stop playback"), &c::misc::movement_rec_keystopplayback);

            ImGui::Text(("clear recording"));
            ImGui::Keybind(("clear recording"), &c::misc::movement_rec_keyclearrecord);
        }


        ImGui::PopFont();
        ImGui::Spacing();
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    //right
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(320);
    ImGui::MenuChild("records settings", ImVec2(285, 540), false); {
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
        ImGui::Spacing();
        ImGui::PushFont(fonts::menu_font_thin);

        mrec->draw();


        ImGui::PopFont();
    }
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::PopFont();
}

void settings_tab() {
    ImGui::PushFont(fonts::menu_font_bold);
    //left upper
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(15);
    ImGui::MenuChild("settings", ImVec2(300, 540), false); {
        ImGui::PushFont(fonts::menu_font_thin);
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
        static int config_index = -1;
        static char buffer[10];

        ImGui::Text(("config list "));
        ImGui::Text((""));
        ImGui::SameLine(group_w - 283);
        if (ImGui::ListBoxHeader(("##configs"), ImVec2(285, 90))) {
            const std::vector<std::string>& configs_vector = c::configs;
            for (std::size_t i = 0u; i < configs_vector.size(); ++i) {
                std::string const& config_name = configs_vector[i].data();
                if (ImGui::Selectable(config_name.c_str(), i == config_index)) {
                    config_index = i;
                }
            }
            ImGui::ListBoxFooter();
        }
        ImGui::Text((""));
        ImGui::SetNextItemWidth(145);
        ImGui::SameLine(group_w - 281);
        ImGui::InputTextWithHint(("##config name"), ("config name"), buffer, sizeof(buffer));

        ImGui::Text((""));
        ImGui::SameLine(group_w - 283);
        if (ImGui::Button(("create config  "))) {
            c::create_file(buffer);
            //features::misc::notify(("created config: ") + (std::string)buffer, color(255, 255, 255, 255));
            std::stringstream ss;

            ss << "\x08" << " \x08"
                << "%c"
                << "delusional"
                << "\x08" << "\x08 | created config " << (std::string)buffer;

            if (interfaces::engine->is_in_game()) {
                interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
            }
        }

        ImGui::SameLine(group_w - 202);
        if (ImGui::Button(("load config  "))) {
            c::load(config_index);
            std::stringstream ss;

            ss << "\x08" << " \x08"
                << "%c"
                << "delusional"
                << "\x08" << "\x08 | loaded config " << c::configs.at(config_index);

            if (interfaces::engine->is_in_game()) {
                interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
                features::skins::forcing_update = true;
            }
            load_i_fonts();
            surface::initialize();
            menu::fonts_initialized = true;
        }
        ImGui::SameLine(group_w - 132);
        if (ImGui::Button(("save config  "))) {
            ImGui::OpenPopup(("config save popup"));
        }
        if (ImGui::BeginPopup(("config save popup"))) {
            ImGui::Text(("are you sure you want to save selected config? "));

            static const char* choices[]{ "  yes", "  no" };

            for (auto i = 0; i < IM_ARRAYSIZE(choices); i++)
                if (ImGui::Selectable(choices[i]))
                    if (i == 0) {
                        c::save(config_index);

                        std::stringstream ss;

                        ss << "\x08" << " \x08"
                            << "%c"
                            << "delusional"
                            << "\x08" << "\x08 | saved config " << c::configs.at(config_index);

                        if (interfaces::engine->is_in_game()) {
                            interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
                        }
                    }

            ImGui::EndPopup();
        }

        ImGui::Text((""));
        ImGui::SameLine(group_w - 283);
        if (ImGui::Button(("refresh config list  "))) {
            c::update_configs();

            std::stringstream ss;
            ss << "\x08" << " \x08"
                << "%c"
                << "delusional"
                << "\x08" << "\x08 | refreshed config list";

            if (interfaces::engine->is_in_game()) {
                interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
            }
        }
        ImGui::SameLine(group_w - 180);
        if (ImGui::Button(("open config folder  "))) {
            ShellExecuteW(0, L"open", L"C:\delusional", NULL, NULL, SW_NORMAL);
        }

        static const char* chat_priont_acc[] = { "default","red","light purple","green","lime","light green","light red","gray","light steel blue","light blue","blue","purple","pink","light steel red","gold","white" };

        ImGui::Text(("menu theme"));
        ImGui::SameLine(group_w - 20);
        ImGui::ColorEdit4(("##main_theme"), menu::menu_accent, no_alpha);
        ImGui::SameLine(group_w - 40);
        ImGui::ColorEdit4(("##s_theme"), menu::menu_accent2, no_alpha);
        ImGui::Text("menu key");
        ImGui::Keybind(("##menu key"), &c::misc::menu_key);
        ImGui::Text("chat print accent");
        ImGui::Combo(("##printacc"), &c::misc::chat_print_clr, chat_priont_acc, IM_ARRAYSIZE(chat_priont_acc));

        ImGui::PushFont(fonts::menu_font_bold);
        ImGui::Text("servers");
        ImGui::PopFont();

        if (ImGui::Button("clarity server  "))
            interfaces::engine->execute_cmd("connect 74.91.113.109:27015");
        ImGui::SameLine();

        if (ImGui::Button("kumori server  "))
            interfaces::engine->execute_cmd("connect 146.19.87.50:27015");
        ImGui::SameLine();

        if (ImGui::Button("breezetix server  "))
            interfaces::engine->execute_cmd("connect 74.91.117.129:27015");

        if (ImGui::Button("saturn server  "))
            interfaces::engine->execute_cmd("connect 209.142.103.1:27075");


        ImGui::PopFont();
    }
    ImGui::EndChild();
    ImGui::EndGroup();

    //right
    ImGui::SetCursorPosY(35);
    ImGui::BeginGroup();
    ImGui::SetCursorPosX(320);
    ImGui::MenuChild("scripts", ImVec2(285, 540), false); {
        float group_w = ImGui::GetCurrentWindow()->Size.x - ImGui::GetStyle().FramePadding.x * 2;
        ImGui::PushFont(fonts::menu_font_thin);

        static int current_lua_item = -1;
        ImGui::Text(("scripts"));
        ImGui::Text((" "));
        ImGui::SameLine(group_w - 269);
        ImGui::ListBoxHeader("##luascripts", ImVec2(272, 90));
        for (int i = 0; i < lua::scripts.size(); i++) {
            const bool is_selected = (current_lua_item == i);
            if (ImGui::Selectable(lua::scripts[i].c_str(), is_selected))
                current_lua_item = i;
        }
        ImGui::ListBoxFooter();

        if (ImGui::Button("refresh list", ImVec2(280, 15))) {
            lua::unload_all_scripts();
            lua::refresh_scripts();

        }

        if (ImGui::Button("reload all scripts", ImVec2(280, 15)))
            lua::reload_all_scripts();

        if (ImGui::Button("unload all scripts", ImVec2(280, 15)))
            lua::unload_all_scripts();

        if (current_lua_item != -1) {
            if (lua::loaded[current_lua_item]) {
                if (ImGui::Button("unload selected", ImVec2(280, 15)))
                    lua::unload_script(current_lua_item);
            }
            else {
                if (ImGui::Button("load selected", ImVec2(280, 15)))
                    lua::load_script(current_lua_item);
            }
        }

        for (auto hk : lua::hook_manager::get_hooks("on_menu")) {
            auto result = hk.func();
            if (!result.valid()) {
                sol::error err = result;
                interfaces::console->console_color_printf({ 255, 0, 0, 255 }, ("[lua] "));
                interfaces::console->console_printf(std::string(err.what()).append(" \n").c_str());
            }
        }
        ImGui::PopFont();
    }
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::PopFont();
}

void menu::render() {
    if (!menu::open) 
        return;

    auto& style = ImGui::GetStyle();
    auto flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(27 / 255.f, 27 / 255.f, 27 / 255.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(menu::menu_accent2[0], menu::menu_accent2[1], menu::menu_accent2[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(24 / 255.f, 24 / 255.f, 24 / 255.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(20 / 255.f, 20 / 255.f, 20 / 255.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(20 / 255.f, 20 / 255.f, 20 / 255.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_SeparatorHovered, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));
    ImGui::PushStyleColor(ImGuiCol_SeparatorActive, ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f));

    ImGui::SetNextWindowSize(ImVec2(620, 614));
    ImGui::Begin("menu", &menu::open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar); {
        style.Alpha = 1.f;

        auto s = ImVec2{}, p = ImVec2{};
        s = ImVec2(ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y * 2); 
        p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y); 
        auto draw = ImGui::GetWindowDrawList();

        draw->AddRectFilled(p, ImVec2(p.x + s.x, p.y + s.y), ImColor(7, 7, 7), 3.f); //tabs bg clr
        draw->AddRectFilled(ImVec2(p.x, p.y + 25), ImVec2(p.x + s.x, p.y + s.y), ImColor(10, 10, 10), 3.f);
        draw->AddRectFilled(ImVec2(p.x, p.y + 25), ImVec2(p.x + s.x, p.y + s.y - 25), ImColor(7, 7, 7), 1.f);// main bg clr
        draw->AddLine(ImVec2(p.x, p.y + 25), ImVec2(p.x + s.x, p.y + 25), ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2])); // tab seperator
        
        draw->AddText(fonts::menu_font_bold, 11.f, ImVec2(p.x + 14, p.y + 4), ImColor(menu::menu_accent2[0], menu::menu_accent2[1], menu::menu_accent2[2]), "delusional");
        draw->AddLine(ImVec2(p.x, p.y + s.y - 25), ImVec2(p.x + s.x, p.y + s.y - 25), ImColor(27, 27, 27)); // bottom seperator

        draw->AddText(fonts::menu_font_thin, 11.f, ImVec2(p.x + 14, p.y + s.y - 20), ImColor(255, 255, 255), "build:");
        draw->AddText(fonts::menu_font_thin, 11.f, ImVec2(p.x + 45, p.y + s.y - 20), ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]), "beta");

        ImGui::SetCursorPos(ImVec2(330, 10));
        ImGui::PushFont(fonts::menu_font_thin);
        ImGui::BeginGroup();
        if (menu::tab("legit", m_tabs == 0))m_tabs = 0; ImGui::SameLine();
        if (menu::tab("visuals", m_tabs == 1))m_tabs = 1; ImGui::SameLine();
        if (menu::tab("misc", m_tabs == 2))m_tabs = 2; ImGui::SameLine();
        if (menu::tab("fonts", m_tabs == 3))m_tabs = 3; ImGui::SameLine();
        if (menu::tab("skins", m_tabs == 4))m_tabs = 4; ImGui::SameLine();
        if (menu::tab("mr", m_tabs == 5))m_tabs = 5; ImGui::SameLine();
        if (menu::tab("other", m_tabs == 6))m_tabs = 6;
        ImGui::EndGroup();
        ImGui::PopFont();

        switch (m_tabs) {
        case 0: aimbot_tab(); break;
        case 1: visuals_tab(); break;
        case 2: misc_tab(); break;
        case 3: fonts_tab(); break;
        case 4: skins_tab(); break;
        case 5: mr_tab(); break;
        case 6: settings_tab(); break;
        }
        ImGui::End();
    }
    ImGui::PopStyleColor(11);

    switch (c::misc::chat_print_clr) {
    case 0: color_p = '\x07'; break;
    case 1: color_p = '\x02'; break;
    case 2: color_p = '\x03'; break;
    case 3: color_p = '\x04'; break;
    case 4: color_p = '\x05'; break;
    case 5: color_p = '\x06'; break;
    case 6: color_p = '\x07'; break;
    case 7: color_p = '\x08'; break;
    case 8: color_p = '\x0A'; break;
    case 9: color_p = '\x0B'; break;
    case 10: color_p = '\x0C'; break;
    case 11: color_p = '\x0D'; break;
    case 12: color_p = '\x0E'; break;
    case 13: color_p = '\x0F'; break;
    case 14: color_p = '\x10'; break;
    case 15: color_p = '\x01'; break;
    }
}