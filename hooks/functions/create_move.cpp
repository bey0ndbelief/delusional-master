#include "../../features/movement/prediction/prediction.hpp"
#include "../../features/movement/movement.hpp"
#include "../../features/scripts/scripts.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/aimbot/aimbot.hpp"
#include "../../features/panorama/scaleform.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"
#include "../../sdk/math/math.hpp"
#include "../../sdk/sdk.hpp"
#include "../hooks.hpp"

bool __fastcall sdk::hooks::create_move::create_move(registers, float sampletime, c_usercmd* cmd ) {
	bool ret = sdk::hooks::create_move::ofunc(ecx, edx, sampletime, cmd);

	if (!cmd || !cmd->command_number)
		return ret;

	g::local = static_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	g::cmd = cmd;

	i_net_channel* net_channel = interfaces::client_state->net_channel;
	vec3_t pre_pred_velocity = g::local->velocity();
	vec3_t pre_pred_origin = g::local->origin();
    int pre_pred_flags = g::local->flags();
	auto current_angle = cmd->view_angles;

	features::movement::auto_align(cmd);
	features::movement::pixel_surf_fix(cmd);
	features::movement::pixel_surf(cmd);

	math::start_movement_fix(cmd);

	features::movement::bhop(cmd);
	features::movement::delay_hop(cmd);
	features::misc::door_spam(cmd);
	features::misc::fix_mouse_delta(cmd);
	features::misc::grenade_trail();
	features::movement::edge_bug_pre_pred(cmd);
	features::visuals::trail();
	features::misc::clantag_spammer();
	features::misc::jumpstats::jumpstats(cmd);
	features::misc::reveal_server_ranks(cmd);
	features::movement::gather_vel_graph_data();
	features::movement::gather_stam_graph_data();
	panorama::scaleform_tick(g::local);

	for (auto hk : lua::hook_manager::get_hooks("on_create_move")) {
		auto result = hk.func(cmd);
		if (!result.valid()) {
			sol::error err = result;
			interfaces::console->console_color_printf({ 255, 0, 0, 255 }, ("[lua] "));
			interfaces::console->console_printf(std::string(err.what()).append(" \n").c_str());
		}
	}

	if (c::movement::fastduck) cmd->buttons |= in_bullrush;

	prediction::begin(cmd); {

		aimbot::run(cmd);
		triggerbot::run(cmd);
		backtrack.on_move(cmd);

	}prediction::end();

	math::end_movement_fix(cmd);

	features::movement::fake_backwards(cmd);

	if (net_channel != nullptr) {
		if (c::backtrack::fake_latency) 
			fakeping.update_sequence(net_channel); 
		else 
			fakeping.clear_sequence();
	}

	features::visuals::run_freecam(cmd, features::movement::old_viewangles);
	features::movement::jump_bug(cmd, pre_pred_flags, pre_pred_velocity);
	features::movement::auto_strafe(cmd, current_angle);
	features::movement::strafe_optimizer(cmd, pre_pred_flags);
	features::movement::edge_jump(cmd, pre_pred_flags);
	features::movement::mini_jump(cmd, pre_pred_flags);
	features::movement::crouch_bug(cmd, pre_pred_flags);
	features::movement::long_jump(cmd);
	features::movement::ladder_jump(cmd);
	features::movement::ladder_bug(cmd);
	features::misc::kz_practice_logic(cmd);
	mrec->hook(cmd, pre_pred_origin);

	vec3_t backup_viewangles = cmd->view_angles;
	features::movement::pixel_surf(cmd);
	features::movement::edge_bug_post_pred(cmd);
	math::start_movement_fix(cmd);
	cmd->view_angles = backup_viewangles;
	math::end_movement_fix(cmd);

	if (c::movement::auto_strafe && menu::checkkey(c::movement::auto_strafe_key, c::movement::auto_strafe_key_s))
		features::movement::fix_mov(cmd, current_angle.y);

	cmd->forward_move = std::clamp(cmd->forward_move, -450.0f, 450.0f);
	cmd->side_move = std::clamp(cmd->side_move, -450.0f, 450.0f);
	cmd->up_move = std::clamp(cmd->up_move, -320.0f, 320.0f);

	math::normalize_angle(cmd->view_angles);
	cmd->view_angles.x = std::clamp(cmd->view_angles.x, -89.0f, 89.0f);
	cmd->view_angles.y = std::clamp(cmd->view_angles.y, -180.0f, 180.0f);
	cmd->view_angles.z = 0.0f;
	features::movement::old_viewangles = cmd->view_angles;

	return false;
}

