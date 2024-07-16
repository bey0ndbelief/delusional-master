#include "misc.hpp"
#include "../../menu/menu.hpp"
#include "../../includes/imgui/imgui.h"
#include "../../includes/imgui/imgui_internal.h"
#include "../../sdk/sdk.hpp"

void features::misc::draw( ) {
	const auto stay_time = 4.f;
	const auto fade_time = 0.6f;

	for (auto i = 0; i < features::misc::notify_list.size(); i++) {
		if (features::misc::notify_list.empty())
			continue;

		notify_t notify = features::misc::notify_list[i];

		float elapsed_time = interfaces::globals->cur_time - (notify.time + stay_time);
		int fade_alpha = static_cast<int>(255.f - (elapsed_time / fade_time) * 255.f);
		fade_alpha = std::clamp(fade_alpha, 0, 255);

		if (fade_alpha <= 0) {
			features::misc::notify_list.erase(features::misc::notify_list.begin() + i);
			i--;
			continue;
		}

		ImGui::PushFont(fonts::logs_font_flag);
		ImGui::GetForegroundDrawList()->AddText(ImVec2(5 + 1, 5 + (i * 15) + 1), ImColor(0, 0, 0, fade_alpha), notify.text.c_str());
		ImGui::GetForegroundDrawList()->AddText(ImVec2(5, 5 + (i * 15)), ImColor(notify._color.r, notify._color.g, notify._color.b, fade_alpha), notify.text.c_str());
		ImGui::PopFont();
	}
}

void features::misc::notify( std::string text, color _color ) {
	if (text.empty()) {
		return;
	}

	features::misc::notify_list.push_back( notify_t( text, color( _color.r, _color.g, _color.b, _color.a ) ) );
}

bool spec_local;
void features::misc::spectators_list() {
	if (!c::misc::spectators_list || !interfaces::engine->is_in_game())
		return;

	int x, y;
	int height = 0;
	int spectator_list_pos_x;
	int spectator_list_pos_y;
	float deltatime = (1.0f / 0.2f) * ImGui::GetIO().DeltaTime * 0.75;
	static float spec_alpha = 1.f;
	int a = spec_alpha * 255;
	interfaces::engine->get_screen_size(x, y);

	std::vector<std::string> spectator_names;
	std::vector<std::string> spectator_targets;
	int spectated_index = -1; // initialize to -1
	for (int i = 0; i < interfaces::globals->max_clients; i++) {
		auto observator = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(i));

		if (!observator || observator->is_alive() || observator->dormant())
			continue;

		if (!c::misc::spectatorlist_show_target)
			if (reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity_handle(observator->observer_target())) != g::local)
				continue;

		auto spectated_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity_handle(observator->observer_target()));

		if (!spectated_player || !spectated_player->is_alive())
			continue;

		player_info_t spectating_info, spectated_info;
		interfaces::engine->get_player_info(observator->index(), &spectating_info);
		interfaces::engine->get_player_info(spectated_player->index(), &spectated_info);

		char spectate_buffer[1024];
		if (c::misc::spectatorlist_show_target) {
			spectator_names.push_back(spectating_info.name);
			spectator_targets.push_back(spectated_info.name);
			sprintf_s(spectate_buffer, ("%s -> %s"), spectating_info.name, spectated_info.name);
		}
		else {
			spectator_names.push_back(spectating_info.name);
			sprintf_s(spectate_buffer, ("%s"), spectating_info.name);
		}

		if (spectated_player == g::local) {
			spectated_index = i; // store the index of the spectated player
		}

		if (c::misc::spectatorlist_type == 0) {
			int y = 5;

			if (c::misc::show_spotify_currently_playing)
				y = 20;

			ImColor spec_clr = ImColor(c::misc::spectators_list_color_2[0], c::misc::spectators_list_color_2[1], c::misc::spectators_list_color_2[2]);
			ImColor local_spec_clr = ImColor(c::misc::spectators_list_color_1[0], c::misc::spectators_list_color_1[1], c::misc::spectators_list_color_1[2]);

			ImGui::GetBackgroundDrawList()->AddText(fonts::main_spec_font, 11.f, ImVec2(x - 5 - im_render.get_text_width(spectate_buffer, fonts::main_spec_font) + 1, (y + 1) + (16 * height)), ImColor(0, 0, 0, 255), spectate_buffer);
			ImGui::GetBackgroundDrawList()->AddText(fonts::main_spec_font, 11.f, ImVec2(x - 5 - im_render.get_text_width(spectate_buffer, fonts::main_spec_font), y + (16 * height)), spectated_player == g::local ? local_spec_clr : spec_clr, spectate_buffer);
			height++;
		}
	}
	/*
	if (c::misc::spectatorlist_type == 1) {
		spec_alpha = ImClamp(spec_alpha + (3.f * deltatime * ((menu::open || spectator_names.size() > 0) ? 1.f : -1.f)), 0.0f, 1.f);

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, spec_alpha);
		ImGui::SetNextWindowSizeConstraints(ImVec2(ImGui::CalcTextSize(("spectators")).x + 20, ImGui::CalcTextSize(("spectators")).y - 10), ImVec2(FLT_MAX, FLT_MAX));
		ImGui::Begin("spectators", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground); {
			auto pos = ImGui::GetWindowPos(); auto size = ImGui::GetWindowSize();

			spectator_list_pos_x = pos.x;
			spectator_list_pos_y = pos.y;
			pos.x = spectator_list_pos_x;
			pos.y = spectator_list_pos_y;

			ImVec2 gradient_start = ImVec2(pos.x, pos.y);
			ImVec2 gradient_end = ImVec2(pos.x + size.x, pos.y + size.y);
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(gradient_start, gradient_end, ImColor(30.f / 255.f, 30.f / 255.f, 30.f / 255.f, spec_alpha), ImColor(30.f / 255.f, 30.f / 255.f, 30.f / 255.f, spec_alpha), ImColor(10.f / 255.f, 10.f / 255.f, 10.f / 255.f, spec_alpha), ImColor(10.f / 255.f, 10.f / 255.f, 10.f / 255.f, spec_alpha));

			float window_width = ImGui::CalcTextSize(("spectators")).x;
			ImGui::GetWindowDrawList()->AddText(fonts::main_spec_font, 11.f, ImVec2(ImGui::GetWindowPos().x + (size.x - window_width) / 2, ImGui::GetWindowPos().y + 5), ImColor(1.f, 1.f, 1.f, spec_alpha), ("spectators"));

			ImGui::SetCursorPos(ImVec2(3, 18));

			for (int i = 0; i < spectator_names.size(); i++) {
				ImGui::SetCursorPosX(3);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.f, 1.f));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.f, 2.f));
				ImGui::PushFont(fonts::spec_font);
				if (c::misc::spectatorlist_show_target) {
					if (i >= spectator_targets.size()) {
						continue; // skip this iteration if spectator_targets does not have enough elements
					}

					auto observator = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(spectated_index));

					if (!observator || observator->is_alive() || observator->dormant())
						continue;

					auto spectated_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity_handle(observator->observer_target()));

					if (!spectated_player || !spectated_player->is_alive())
						continue;

					player_info_t spectating_info, spectated_info;
					interfaces::engine->get_player_info(observator->index(), &spectating_info);
					interfaces::engine->get_player_info(spectated_player->index(), &spectated_info);

					std::string spectator_info = spectator_names.at(i) + " -> " + spectator_targets.at(i);
					ImGui::TextColored(spectated_player == g::local ? ImColor(c::misc::spectators_list_color_1[0], c::misc::spectators_list_color_1[1], c::misc::spectators_list_color_1[2]) : ImColor(c::misc::spectators_list_color_2[0], c::misc::spectators_list_color_2[1], c::misc::spectators_list_color_2[2]), spectator_info.c_str());
				}
				else {
					ImGui::Text(spectator_names.at(i).c_str());
				}
				ImGui::PopFont();
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
			}

		}
		ImGui::End();
		ImGui::PopStyleVar();
	}
	*/
}

vec3_t origin;
float pressdelay = 0.f;
int currentcheckpoint = 0;
std::deque<std::pair<vec3_t, vec3_t>> checkpoints;
void features::misc::kz_practice_logic(c_usercmd* cmd) {
	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game() || !c::misc::practice) {
		checkpoints.clear();
		currentcheckpoint = 0;
		pressdelay = 0;
		return;
	}

	float j = interfaces::console->get_convar("sv_cheats")->get_float();
	if (j == 1) {
		if (g::local->is_alive() && pressdelay < interfaces::globals->realtime) {
			std::stringstream ss;
			if (GetAsyncKeyState(c::misc::savepos) && (g::local->flags() & fl_onground)) {
				checkpoints.push_front({ vec3_t(cmd->view_angles.x, cmd->view_angles.y, 0.f), local_player->origin() });
				currentcheckpoint = 0;
				if (checkpoints.size() > 99)
					checkpoints.pop_back();
				pressdelay = interfaces::globals->realtime + 0.2;
				for (int i = 1; i <= 1; ++i) {
					ss << "\x08" << " \x08" << "%c" << "delusional" << "\x08" << "\x08 | saved checkpoint.";
					interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
				}
			}
			else if (GetAsyncKeyState(c::misc::savepos) && !(g::local->flags() & fl_onground)){
				pressdelay = interfaces::globals->realtime + 0.2;
				for (int i = 1; i == 1; ++i) {
					ss << "\x08" << " \x08" << "%c" << "delusional" << "\x08" << "\x08 | failed to save position";
					interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
				}
			}

			if (!checkpoints.empty()) {
				if (menu::checkkey(c::misc::loadpos, c::misc::loadpos_s)) {
					auto& cur = checkpoints.at(currentcheckpoint);
					std::string cmd = ("setpos ") + std::to_string(cur.second.x) + (" ") + std::to_string(cur.second.y) + (" ") + std::to_string(cur.second.z) + (";");
					interfaces::engine->execute_cmd(cmd.c_str());
					cmd = ("setang ") + std::to_string(cur.first.x) + (" ") + std::to_string(cur.first.y) + (" ") + std::to_string(cur.first.z) + (";");
					interfaces::engine->execute_cmd(cmd.c_str());
					pressdelay = interfaces::globals->realtime + 0.2;
					for (int i = 1; i <= 1; ++i) {
						ss << "\x08" << " \x08" << "%c" << "delusional" << "\x08" << "\x08 | teleported to saved checkpoint.";
						interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
					}
				}
				if (menu::checkkey(c::misc::nextkey, c::misc::nextkey_s)) {
					if (currentcheckpoint > 0)
						currentcheckpoint--;
					pressdelay = interfaces::globals->realtime + 0.2;
					for (int i = 1; i <= 1; ++i) {
						ss << "\x08" << " \x08" << "%c" << "delusional" << "\x08" << "\x08 | next position.";
						interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
					}
				}
				if (menu::checkkey(c::misc::prevkey, c::misc::prevkey_s)) {
					if (currentcheckpoint + 1 < checkpoints.size())
						currentcheckpoint++;
					pressdelay = interfaces::globals->realtime + 0.2;
					for (int i = 1; i <= 1; ++i) {
						ss << "\x08" << " \x08" << "%c" << "delusional" << "\x08" << "\x08 | previous position.";
						interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
					}
				}
				if (menu::checkkey(c::misc::undokey, c::misc::undokey_s)) {
					checkpoints.erase(checkpoints.begin() + currentcheckpoint);
					if (currentcheckpoint >= checkpoints.size())
						currentcheckpoint = max(0, checkpoints.size() - 1);
					pressdelay = interfaces::globals->realtime + 0.2;
					for (int i = 1; i <= 1; ++i) {
						ss << "\x08" << " \x08" << "%c" << "delusional" << "\x08" << "\x08 | undid position.";
						interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
					}
				}
			}
		}
	}
}

void features::misc::kz_practice() {
	if (!c::misc::practice_window )
		return;

}

void features::misc::reveal_server_ranks(c_usercmd * cmd) {
	if (!c::misc::misc_reveal_ranks || !interfaces::engine->is_in_game())
		return;

	if (cmd->buttons & in_score)
		interfaces::client->dispatch_user_message(50, 0, 0, nullptr);
}

void features::misc::thirdperson() {

}

void features::misc::reset_thirdperson() {

}

void features::misc::view_model() {
	if (c::misc::view_model) {
		if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
			return;

		if (!g::local || !g::local->is_alive())
			return;

		const auto view_model = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity_handle(g::local->view_model()));

		if (!view_model)
			return;

		vec3_t forward{}, right{}, up{};
		math::angle_vectors_alternative(view_model->abs_angles(), &forward, &right, &up);

		view_model->set_abs_origin(view_model->abs_origin() + forward * c::misc::view_model_z + up * c::misc::view_model_y + right * c::misc::view_model_x);
	}

	//

	static auto default_scale = interfaces::console->get_convar("cl_wpn_sway_scale")->get_float();
	static auto* swayscale = interfaces::console->get_convar("cl_wpn_sway_scale");

	swayscale->set_value(c::misc::swayscale ? c::misc::swayscale_value : default_scale);
}

void features::misc::door_spam(c_usercmd* cmd) {
	if (!c::misc::door_spam || !menu::checkkey(c::misc::door_spam_key, c::misc::door_spam_key_s))
		return;

	static bool spam = false;
	static float time = 0;
	if (interfaces::globals->cur_time > time) {

		cmd->buttons &= in_use;

		time = interfaces::globals->cur_time + 0.2f;
	}
	if (fabs(time - interfaces::globals->cur_time) > 0.3f)
		time = interfaces::globals->cur_time;
}

void features::misc::fix_mouse_delta(c_usercmd* cmd) {
	if (!c::misc::mouse_fix)
		return;

	static vec3_t delta_viewangles{ };
	vec3_t delta = cmd->view_angles - delta_viewangles;
	delta.clamp();

	static convar* sensitivity = interfaces::console->get_convar("sensitivity");

	if (!sensitivity)
		return;

	if (delta.x != 0.f) {
		static convar* m_pitch = interfaces::console->get_convar("m_pitch");

		if (!m_pitch)
			return;

		int final_dy = static_cast<int>((delta.x / m_pitch->get_float()) / sensitivity->get_float());
		if (final_dy <= 32767) {
			if (final_dy >= -32768) {
				if (final_dy >= 1 || final_dy < 0) {
					if (final_dy <= -1 || final_dy > 0)
						final_dy = final_dy;
					else
						final_dy = -1;
				}
				else {
					final_dy = 1;
				}
			}
			else {
				final_dy = 32768;
			}
		}
		else {
			final_dy = 32767;
		}

		cmd->mouse_dy = static_cast<short>(final_dy);
	}

	if (delta.y != 0.f) {
		static convar* m_yaw = interfaces::console->get_convar("m_yaw");

		if (!m_yaw)
			return;

		int final_dx = static_cast<int>((delta.y / m_yaw->get_float()) / sensitivity->get_float());
		if (final_dx <= 32767) {
			if (final_dx >= -32768) {
				if (final_dx >= 1 || final_dx < 0) {
					if (final_dx <= -1 || final_dx > 0)
						final_dx = final_dx;
					else
						final_dx = -1;
				}
				else {
					final_dx = 1;
				}
			}
			else {
				final_dx = 32768;
			}
		}
		else {
			final_dx = 32767;
		}

		cmd->mouse_dx = static_cast<short>(final_dx);
	}

	delta_viewangles = cmd->view_angles;
}

void features::misc::radaringame(player_t* p) {
	

}

void apply_clan_tag(const char* tag, const char* name) {
	using fn = int(__fastcall*)(const char*, const char*);
	static auto apply_clan_tag_fn = reinterpret_cast<fn>(find_pattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
	apply_clan_tag_fn(tag, name);
};

void features::misc::clantag_spammer() {
	if (!c::misc::misc_clantag_spammer || !interfaces::engine->is_in_game())
		return;

	static bool reset = true;
	static auto lasttime = 0.0f;
	static std::string torotate = c::misc::misc_clantag_text;
	static std::string rotating = c::misc::misc_clantag_text;

	if (c::misc::misc_clantag_type == 0) {
		if (interfaces::globals->realtime - lasttime < 1.0f)
			return;

		apply_clan_tag(xs(""), xs(""));
		lasttime = interfaces::globals->realtime;
	}
	else if (c::misc::misc_clantag_type == 1) {
		if (c::misc::misc_animated_clantag) {
			if (interfaces::globals->realtime - lasttime < c::misc::misc_clantag_speed)
				return;

			if (torotate != std::string(c::misc::misc_clantag_text)) {
				torotate = c::misc::misc_clantag_text;
				rotating = c::misc::misc_clantag_text;
			}

			if (c::misc::misc_clantag_rotation) {
				if (!rotating.empty()) {
					char last = rotating.back();
					rotating.pop_back();
					rotating.insert(rotating.begin(), last);
					apply_clan_tag(rotating.c_str(), torotate.c_str());
				}
			}
			else {
				if (!rotating.empty()) {
					std::rotate(rotating.begin(), rotating.begin() + (rotating.size() - 1), rotating.end());
					apply_clan_tag(rotating.c_str(), torotate.c_str());
				}
			}

			lasttime = interfaces::globals->realtime;
		}
		else {
			if (interfaces::globals->realtime - lasttime < 1.0f)
				return;

			apply_clan_tag(c::misc::misc_clantag_text, c::misc::misc_clantag_text);
			lasttime = interfaces::globals->realtime;
		}
	}
}

inline int hitmarker_time = 0;
void features::misc::hitmarker::event(i_game_event* event) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!g::local || !g::local->is_alive())
		return;

	auto event_name = fnv::hash(event->get_name());
	auto attacker = interfaces::ent_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));

	if (!attacker || !event_name)
		return;

	switch (event_name) {
	case fnv::hash("player_hurt"):
		if (attacker == g::local) {
			hitmarker_time = 255;

			if (c::misc::misc_hitmarker_sound) {

				switch (c::misc::misc_hitmarker_sound_type) {
				case 0: interfaces::surface->play_sound("buttons\\arena_switch_press_02.wav"); break;
				case 1: interfaces::surface->play_sound("buttons\\button22.wav"); break;
				case 2: interfaces::surface->play_sound("survival\\money_collect_01.wav"); break;
				case 3: interfaces::surface->play_sound("Ui\\beep07.wav"); break;
				}
			}
		}
		break;
	}
}

const char* hitgroup_name(int hitgroup) {
	switch (hitgroup) {
	case hitgroup_head:
		return "head";
	case hitgroup_leftleg:
		return "left leg";
	case hitgroup_rightleg:
		return "right leg ";
	case hitgroup_stomach:
		return "stomach";
	case hitgroup_leftarm:
		return "left arm";
	case hitgroup_rightarm:
		return "right arm";
	default:
		return "body";
	}
}

void features::misc::hit_info(i_game_event* event) {

	if (!c::misc::misc_hit_info || !interfaces::engine->is_in_game())
		return;

	if (!event)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	auto victim = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
	if (!victim)
		return;

	auto attacker = interfaces::ent_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	if (!attacker)
		return;

	if (attacker == local_player) {
		auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(victim));
		if (!entity) {
			return;
		}

		player_info_t info;
		interfaces::engine->get_player_info(entity->index(), &info);

		auto damage = event->get_int("dmg_health");
		if (!damage)
			return;

		auto health = event->get_int("health");
		if (!health && health != 0)
			return;

		auto hitbox = event->get_int("hitgroup");
		if (!hitbox) {
			std::string player_name = info.name;
			if (c::misc::misc_hit_info[0])
			    interfaces::chat_element->chatprintf(0, 0, " %c delusional \x08| hit %s for %d damage (%d remaining). \n", menu::color_p, player_name.c_str(), damage, health);

			if (c::misc::misc_hit_info[1])
			    features::misc::notify((std::stringstream{ } << "hit " << player_name.c_str() << " for " << damage << " damage (" << health << " health remaining).").str().c_str(), color(255, 255, 255, 255));
			return;
		}

		auto hitgroup = hitgroup_name(hitbox);

		std::string player_name = info.name;

		if (c::misc::misc_hit_info[0])
		    interfaces::chat_element->chatprintf(0, 0, " %c delusional \x08| hit %s in the %s for %d damage (%d remaining). \n", menu::color_p, player_name.c_str(), hitgroup, damage, health);

		if (c::misc::misc_hit_info[1])
		    features::misc::notify((std::stringstream{ } << "hit " << player_name.c_str() << " in the " << hitgroup << " for " << damage << " damage (" << health << " health remaining).").str().c_str(), color(255, 255, 255, 255));

	}
}

void features::misc::hitmarker::draw() {
	if (!c::misc::misc_hitmarker)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!g::local || !g::local->is_alive())
		return;

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	if (hitmarker_time > 0) {
		float alpha = hitmarker_time;

		surface::line((width / 2) + 6, (height / 2) + 6, (width / 2) + 3, (height / 2) + 3, color::white(alpha));
		surface::line((width / 2) - 6, (height / 2) + 6, (width / 2) - 3, (height / 2) + 3, color::white(alpha));
		surface::line((width / 2) + 6, (height / 2) - 6, (width / 2) + 3, (height / 2) - 3, color::white(alpha));
		surface::line((width / 2) - 6, (height / 2) - 6, (width / 2) - 3, (height / 2) - 3, color::white(alpha));

		hitmarker_time -= 2;
	}
}

void features::misc::kill_say(i_game_event* event) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!g::local || !g::local->is_alive())
		return;

	auto event_name = fnv::hash(event->get_name());
	auto attacker = interfaces::ent_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("userid"))));

	if (!attacker || !entity || !event_name)
		return;

	switch (event_name) {
	case fnv::hash("player_death"):
		if (attacker == g::local && entity != g::local) {

			if (c::misc::misc_kill_msg)
				interfaces::engine->execute_cmd((std::string("say ").append(c::misc::misc_kill_message)).c_str());

			if (c::misc::misc_hitmarker_screen_effect)
				g::local->health_boost_time() = interfaces::globals->cur_time + 0.4f;
		}
		break;
	}
}

void features::misc::grenade_trail() {
	static auto nade{ interfaces::console->get_convar("cl_grenadepreview") };
	nade->callbacks.size = 0;
	nade->set_value(c::misc::nadepred);
}

void features::misc::force_crosshair() {
	const static auto weapon_debug_spread_show = interfaces::console->get_convar(xs("weapon_debug_spread_show"));

	const auto should_draw_crosshair = c::misc::force_crosshair && g::local &&
		g::local->is_alive() &&
		!g::local->is_scoped();

	weapon_debug_spread_show->set_value(should_draw_crosshair ? 3 : 0);
}

void features::misc::sniper_crosshair() {
	if (!c::misc::sniper_crosshair)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!g::local || !g::local->is_alive())
		return;

	const auto weapon = g::local->active_weapon();

	if (!weapon) 
		return;

	const auto data = interfaces::weapon_system->get_weapon_data(weapon->item_definition_index());

	if (!data) 
		return;

	if (data->m_iWeaponType != WEAPONTYPE_SNIPER_RIFLE)
		return;

	if (g::local->is_scoped()) 
		return;

	int x, y;
	interfaces::engine->get_screen_size(x, y);

	im_render.drawrectfilled(x / 2, y / 2, 2, 2, color_t(255, 255, 255));
}

void features::misc::vote_revealer(i_game_event* event) {
	if (!c::misc::vote_revealer || !interfaces::engine->is_in_game())
		return;

	int vote = event->get_int("vote_option");
	int id = event->get_int("entityid");
	player_info_t player;
	interfaces::engine->get_player_info(id, &player);
	interfaces::chat_element->chatprintf(0, 0, " \x07 %s\x08 voted %s\x08.\n", player.name, vote == 0 ? "\x07yes" : "\x02no");
}

