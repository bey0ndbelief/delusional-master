#define NOMINMAX
#include "movement.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/config/config.hpp"
#include "../movement/prediction/prediction.hpp"

void features::movement::bhop(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::bhop) {
		if (!GetAsyncKeyState(c::movement::jump_bug_key) && g::local->move_type() != movetype_ladder)
			if (!(g::local->flags() & fl_onground) && cmd->buttons & (in_jump)) {
				cmd->buttons &= ~(in_jump);
			}
	}
}

void features::movement::delay_hop(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::delay_hop && menu::checkkey(c::movement::delay_hop_key, c::movement::delay_hop_key_s)) {

		static int ticks = 0;
		const int ticks_to_wait = c::movement::dh_tick;

		const int move_type = g::local->move_type();
		if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
			return;

		if (g::local->flags() & fl_onground) {
			if (cmd->tick_count > ticks + ticks_to_wait) {
				cmd->buttons |= in_jump;
				ticks = cmd->tick_count;
			}
		}
		else {
			ticks = cmd->tick_count;
		}
	}
}

void features::movement::crouch_bug(c_usercmd* cmd, int pre_pred_flags) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::crouch_bug && menu::checkkey(c::movement::crouch_bug_key, c::movement::crouch_bug_key_s)) {
		if (!(pre_pred_flags & (fl_onground)) && g::local->flags() & (fl_onground)) {
			cmd->buttons |= (in_duck);
		}
	}
}

void features::movement::edge_jump(c_usercmd* cmd, int pre_flags) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	const int move_type = g::local->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) {
		return;
	}

	if (c::movement::edge_jump && menu::checkkey(c::movement::edge_jump_key, c::movement::edge_jump_key_s)) {
		if (pre_flags & (fl_onground) && !(g::local->flags() & fl_onground)) {
			if (c::movement::adaptive_key_cancelling && c::movement::adaptive_key_for[0]) {
				if (c::movement::lj_null[0]) {
					cmd->buttons &= ~in_forward;
					interfaces::engine->execute_cmd(xs("-forward"));
				}
				if (c::movement::lj_null[1]) {
					cmd->buttons &= ~in_back;
					interfaces::engine->execute_cmd(xs("-back"));
				}
				if (c::movement::lj_null[2]) {
					cmd->buttons &= ~in_moveleft;
					interfaces::engine->execute_cmd(xs("-moveleft"));
				}
				if (c::movement::lj_null[3]) {
					cmd->buttons &= ~in_moveright;
					interfaces::engine->execute_cmd(xs("-moveright"));

				}
			}
			should_ej = true;
			cmd->buttons |= in_jump;
		}
		else {
			should_ej = false;
		}
	}
}

static int saved_tick_count;
void features::movement::long_jump(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::long_jump_on_edge) {
		const int move_type = g::local->move_type();
		if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) {
			return;
		}
		if (g::local->flags() & fl_onground) {
			saved_tick_count = interfaces::globals->tick_count;
		}
		if (interfaces::globals->tick_count - saved_tick_count > 2) {
			do_long_jump = false;
		}
		else {
			do_long_jump = true;
		}
		if (menu::checkkey(c::movement::long_jump_key, c::movement::long_jump_key_s)) {
			if (do_long_jump && !(g::local->flags() & fl_onground)) {
				if (c::movement::adaptive_key_cancelling && c::movement::adaptive_key_for[3]) {
					if (c::movement::lj_null[0]) {
						cmd->buttons &= ~in_forward;
						interfaces::engine->execute_cmd(xs("-forward"));
					}
					if (c::movement::lj_null[1]) {
						cmd->buttons &= ~in_back;
						interfaces::engine->execute_cmd(xs("-back"));
					}
					if (c::movement::lj_null[2]) {
						cmd->buttons &= ~in_moveleft;
						interfaces::engine->execute_cmd(xs("-moveleft"));
					}
					if (c::movement::lj_null[3]) {
						cmd->buttons &= ~in_moveright;
						interfaces::engine->execute_cmd(xs("-moveright"));
					}
				}
				cmd->buttons |= in_duck;
				should_lj = true;
			}
			else {
				should_lj = false;
			}
		}
	}
}

static int saved_tick_count_mj;
void features::movement::mini_jump(c_usercmd* cmd, int pre_pred_flags) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	const int move_type = g::local->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) {
		return;
	}

	if (c::movement::mini_jump && menu::checkkey(c::movement::mini_jump_key, c::movement::mini_jump_key_s)) {
		if (pre_pred_flags & (fl_onground) && !(g::local->flags() & fl_onground)) {
			if (c::movement::adaptive_key_cancelling && c::movement::adaptive_key_for[1]) {
				cmd->buttons &= ~in_forward;
				interfaces::engine->execute_cmd(xs("-forward"));
			}
			if (c::movement::lj_null[1]) {
				cmd->buttons &= ~in_back;
				interfaces::engine->execute_cmd(xs("-back"));
			}
			if (c::movement::lj_null[2]) {
				cmd->buttons &= ~in_moveleft;
				interfaces::engine->execute_cmd(xs("-moveleft"));
			}
			if (c::movement::lj_null[3]) {
				cmd->buttons &= ~in_moveright;
				interfaces::engine->execute_cmd(xs("-moveright"));
			}
			should_mj = true;

			if (c::movement::mini_jump_ej)
				cmd->buttons |= in_jump;

			cmd->buttons |= in_duck;
		}
		else {
			should_mj = false;
		}
	}
}

void features::movement::ladder_bug(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::ladder_bug) {

		if (!menu::checkkey(c::movement::ladder_bug_key, c::movement::ladder_bug_key_s))
			return;

		if (g::local->move_type() & movetype_ladder) {
			cmd->up_move = 450.f;
			cmd->buttons |= in_jump;
			should_lb = true;
		}
		else {
			should_lb = false;
		}
	}
}

void features::movement::ladder_jump(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	static int saved_tick;

	if (c::movement::edge_jump && menu::checkkey(c::movement::edge_jump_key, c::movement::edge_jump_key_s)) {
		if (c::movement::edge_jump_on_ladder) {
			const int pre = g::local->move_type();

			prediction::begin(cmd);
			prediction::end();

			const int post = g::local->move_type();
			if (pre == movetype_ladder) {
				if (post != movetype_ladder) {
					saved_tick = interfaces::globals->tick_count;
					cmd->buttons |= in_jump;
					cmd->forward_move = 0.f;
					cmd->side_move = 0.f;
					cmd->buttons = cmd->buttons & ~(in_forward | in_back | in_moveright | in_moveleft);
				}
			}

			if (interfaces::globals->tick_count - saved_tick > 1 && interfaces::globals->tick_count - saved_tick < 15) {
				cmd->forward_move = 0.f;
				cmd->side_move = 0.f;
				cmd->buttons = cmd->buttons & ~(in_forward | in_back | in_moveright | in_moveleft);
				cmd->buttons |= in_duck;
			}
		}
	}
}

void features::movement::blockbot(c_usercmd* cmd) {

}

void features::movement::auto_strafe(c_usercmd* cmd, vec3_t& current_angle) {
	if (!c::movement::auto_strafe || !menu::checkkey(c::movement::auto_strafe_key, c::movement::auto_strafe_key_s))
		return;

	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip)
		return;

	if (g::local->flags() & fl_onground)
		return;

	if (g::local->get_velocity().length_2d() <= 5.f)
		return;

	static float angle = 0.f;

	const bool back = cmd->buttons & in_back;
	const bool forward = cmd->buttons & in_forward;
	const bool right = cmd->buttons & in_moveright;
	const bool left = cmd->buttons & in_moveleft;

	if (back) {
		angle = -180.f;
		if (left)
			angle -= 45.f;
		else if (right)
			angle += 45.f;
	}
	else if (left) {
		angle = 90.f;
		if (back)
			angle += 45.f;
		else if (forward)
			angle -= 45.f;
	}
	else if (right) {
		angle = -90.f;
		if (back)
			angle -= 45.f;
		else if (forward)
			angle += 45.f;
	}
	else {
		angle = 0.f;
	}

	current_angle.y += angle;

	cmd->forward_move = 0.f;
	cmd->side_move = 0.f;

	auto velocity = g::local->get_velocity();
	const auto delta = math::normalize_yaw(current_angle.y - math::rad2deg(std::atan2(velocity.y, velocity.x)));

	cmd->side_move = delta > 0.f ? -450.f : 450.f;

	current_angle.y = math::normalize_yaw(current_angle.y - delta);
}

void features::movement::fix_mov(c_usercmd* cmd, float yaw) {
	float old_yaw = yaw + (yaw < 0.0f ? 360.0f : 0.0f);
	float new_yaw = cmd->view_angles.y + (cmd->view_angles.y < 0.0f ? 360.0f : 0.0f);
	float yaw_delta = new_yaw < old_yaw ? fabsf(new_yaw - old_yaw) : 360.0f - fabsf(new_yaw - old_yaw);
	yaw_delta = 360.0f - yaw_delta;

	const float forwardmove = cmd->forward_move;
	const float sidemove = cmd->side_move;
	cmd->forward_move = std::cos(math::deg2rad(yaw_delta)) * forwardmove + std::cos(math::deg2rad(yaw_delta + 90.0f)) * sidemove;
	cmd->side_move = std::sin(math::deg2rad(yaw_delta)) * forwardmove + std::sin(math::deg2rad(yaw_delta + 90.0f)) * sidemove;
}

void features::movement::movement_correction(c_usercmd* cmd, vec3_t old_angles, vec3_t wish_angle, player_t* local_player) {
	if (old_angles.x != wish_angle.x || old_angles.y != wish_angle.y || old_angles.z != wish_angle.z) {
		vec3_t wish_forward, wish_right, wish_up, cmd_forward, cmd_right, cmd_up;

		auto view_point = old_angles;
		auto movedata = vec3_t(cmd->forward_move, cmd->side_move, cmd->up_move);
		view_point.normalize();

		if (!(local_player->flags() & fl_onground) && view_point.z != 0.f)
			movedata.y = 0.f;

		math::angle_vectors(wish_angle, &wish_forward, &wish_right, &wish_up);
		math::angle_vectors(view_point, &cmd_forward, &cmd_right, &cmd_up);

		auto v8 = sqrt(wish_forward.x * wish_forward.x + wish_forward.y * wish_forward.y), v10 = sqrt(wish_right.x * wish_right.x + wish_right.y * wish_right.y), v12 = sqrt(wish_up.z * wish_up.z);

		vec3_t wish_forward_norm(1.0f / v8 * wish_forward.x, 1.0f / v8 * wish_forward.y, 0.f),
			wish_right_norm(1.0f / v10 * wish_right.x, 1.0f / v10 * wish_right.y, 0.f),
			wish_up_norm(0.f, 0.f, 1.0f / v12 * wish_up.z);

		auto v14 = sqrt(cmd_forward.x * cmd_forward.x + cmd_forward.y * cmd_forward.y), v16 = sqrt(cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y), v18 = sqrt(cmd_up.z * cmd_up.z);

		vec3_t cmd_forward_norm(1.0f / v14 * cmd_forward.x, 1.0f / v14 * cmd_forward.y, 1.0f / v14 * 0.0f),
			cmd_right_norm(1.0f / v16 * cmd_right.x, 1.0f / v16 * cmd_right.y, 1.0f / v16 * 0.0f),
			cmd_up_norm(0.f, 0.f, 1.0f / v18 * cmd_up.z);

		auto v22 = wish_forward_norm.x * movedata.x, v26 = wish_forward_norm.y * movedata.x, v28 = wish_forward_norm.z * movedata.x, v24 = wish_right_norm.x * movedata.y, v23 = wish_right_norm.y * movedata.y, v25 = wish_right_norm.z * movedata.y, v30 = wish_up_norm.x * movedata.z, v27 = wish_up_norm.z * movedata.z, v29 = wish_up_norm.y * movedata.z;

		vec3_t correct_movement;
		correct_movement.x = cmd_forward_norm.x * v24 + cmd_forward_norm.y * v23 + cmd_forward_norm.z * v25 + (cmd_forward_norm.x * v22 + cmd_forward_norm.y * v26 + cmd_forward_norm.z * v28) + (cmd_forward_norm.y * v30 + cmd_forward_norm.x * v29 + cmd_forward_norm.z * v27);
		correct_movement.y = cmd_right_norm.x * v24 + cmd_right_norm.y * v23 + cmd_right_norm.z * v25 + (cmd_right_norm.x * v22 + cmd_right_norm.y * v26 + cmd_right_norm.z * v28) + (cmd_right_norm.x * v29 + cmd_right_norm.y * v30 + cmd_right_norm.z * v27);
		correct_movement.z = cmd_up_norm.x * v23 + cmd_up_norm.y * v24 + cmd_up_norm.z * v25 + (cmd_up_norm.x * v26 + cmd_up_norm.y * v22 + cmd_up_norm.z * v28) + (cmd_up_norm.x * v30 + cmd_up_norm.y * v29 + cmd_up_norm.z * v27);

		static convar* cl_forwardspeed = interfaces::console->get_convar("cl_forwardspeed");

		if (cl_forwardspeed == nullptr)
			return;

		static convar* cl_sidespeed = interfaces::console->get_convar("cl_sidespeed");

		if (cl_sidespeed == nullptr)
			return;

		static convar* cl_upspeed = interfaces::console->get_convar("cl_upspeed");

		if (cl_upspeed == nullptr)
			return;

		const float max_forward_speed = cl_forwardspeed->get_float();
		const float max_side_speed = cl_sidespeed->get_float();
		const float max_up_speed = cl_upspeed->get_float();

		correct_movement.x = std::clamp(correct_movement.x, -max_forward_speed, max_forward_speed);
		correct_movement.y = std::clamp(correct_movement.y, -max_side_speed, max_side_speed);
		correct_movement.z = std::clamp(correct_movement.z, -max_up_speed, max_up_speed);

		cmd->forward_move = correct_movement.x;
		cmd->side_move = correct_movement.y;
		cmd->up_move = correct_movement.z;

		cmd->buttons &= ~(in_moveright | in_moveleft | in_back | in_forward);
		if (cmd->side_move != 0.0) {
			if (cmd->side_move <= 0.0)
				cmd->buttons |= in_moveleft;
			else
				cmd->buttons |= in_moveright;
		}

		if (cmd->forward_move != 0.0) {
			if (cmd->forward_move <= 0.0)
				cmd->buttons |= in_back;
			else
				cmd->buttons |= in_forward;
		}
	}
}

void features::movement::strafe_optimizer(c_usercmd* cmd, int pre_flags) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::strafe_optimizer) {
		if (menu::checkkey(c::misc::freecam_key, c::misc::freecam_key_s))
			return;

		const int move_type = g::local->move_type();

		if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
			return;

		static auto m_yaw = interfaces::console->get_convar("m_yaw");
		static auto sensitivity = interfaces::console->get_convar("sensitivity");
		static float previous_view_angles_yaw = cmd->view_angles.y;

		if (cmd->forward_move == 0 && !(g::local->flags() & fl_onground)) if (auto velocity = g::local->velocity(); velocity.length_2d() > c::movement::strafe_optimizer_required_speed) {
			interfaces::prediction->update(interfaces::client_state->delta_tick,
				interfaces::client_state->delta_tick > 0, interfaces::client_state->last_command_ack,
				interfaces::client_state->last_outgoing_command + interfaces::client_state->choked_commands);

			float mouse_yaw_factor = m_yaw->get_float();
			float mouse_sensitivity = sensitivity->get_float();
			float mouse_yaw_step = mouse_sensitivity * mouse_yaw_factor;

			if (cmd->side_move < 0) {
				if (previous_view_angles_yaw - cmd->view_angles[1] < 0) {
					float strafe_angle = remainderf(cmd->view_angles[1] - atan2f(velocity[1], velocity[0]) * 180 / m_pi, 360) * c::movement::strafe_optimizer_desired_gain / 100;
					if (strafe_angle < -mouse_yaw_step) {
						if (strafe_angle < -180) {
							strafe_angle = -180;
						}
						cmd->view_angles[1] = remainderf(cmd->view_angles[1] - mouse_yaw_step * roundf(strafe_angle / mouse_yaw_step), 360);
						cmd->mouse_dx = (__int16)(mouse_sensitivity * ceilf(remainderf(previous_view_angles_yaw - cmd->view_angles[1], 360) / sqrtf(mouse_yaw_step)));
						interfaces::engine->set_view_angles(cmd->view_angles);
					}
				}
			}
			else {
				if (cmd->side_move > 0) {
					if (previous_view_angles_yaw - cmd->view_angles[1] > 0) {
						float strafe_angle = remainderf(cmd->view_angles[1] - atan2f(velocity[1], velocity[0]) * 180 / m_pi, 360) * c::movement::strafe_optimizer_desired_gain / 100;
						if (strafe_angle > mouse_yaw_step) {
							if (strafe_angle > 180) {
								strafe_angle = 180;
							}
							cmd->view_angles[1] = remainderf(cmd->view_angles[1] - mouse_yaw_step * roundf(strafe_angle / mouse_yaw_step), 360);
							cmd->mouse_dx = (__int16)(mouse_sensitivity * ceilf(remainderf(previous_view_angles_yaw - cmd->view_angles[1], 360) / sqrtf(mouse_yaw_step)));
							interfaces::engine->set_view_angles(cmd->view_angles);

						}
					}
				}
			}
		}
		previous_view_angles_yaw = cmd->view_angles[1];
	}
}

void features::movement::jump_bug( c_usercmd* cmd, int pre_flags, vec3_t pre_pred_vel ) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::jump_bug ) {

		if (!menu::checkkey(c::movement::jump_bug_key, c::movement::jump_bug_key_s))
			return;

		if (!(pre_flags & (fl_onground)) && g::local->flags() & (fl_onground)) {
			cmd->buttons |= in_duck;
		}

		if (g::local->flags() & fl_onground) {
			cmd->buttons &= ~in_jump;
		}

		detected_normal_jump_bug = (c::movement::jump_bug
			&& !should_edge_bug
			&& g::local->velocity().z > pre_pred_vel.z
			&& !(pre_flags & 1 )
			&& !( g::local->flags() & 1 )
			&& g::local->move_type() != movetype_ladder
			&& g::local->move_type() != movetype_noclip
			&& g::local->move_type() != movetype_observer) ? true : false;

		if (detected_normal_jump_bug && c::movement::jump_bug_detection_printf) {
			std::stringstream ss;

			ss << "\x08" << " \x08"
				<< "%c"
				<< "delusional"
				<< "\x08" << "\x08 | jumpbugged";

			interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
		}
	}
}

bool features::movement::check_edge_bug(c_usercmd* cmd) {
	static convar* gravity = interfaces::console->get_convar(("sv_gravity"));
	float edge_bug_vel = (gravity->get_float() * 0.5f * interfaces::globals->interval_per_tick);

	if (c::movement::edge_bug_advanced_search) {
		vec3_t player_origin = g::local->origin();
		trace_t trace;
		ray_t ray;
		trace_world_only filter;
		filter.p_skip1 = g::local;

		vec3_t start = vec3_t(player_origin.x, player_origin.y, player_origin.z + 1);
		vec3_t end = vec3_t(player_origin.x, player_origin.y, player_origin.z - 100);
		ray.initialize(start, end);
		interfaces::trace_ray->trace_ray(ray, CONTENTS_PLAYERCLIP, &filter, &trace);

		vec3_t ground_origin = trace.end;
		vec3_t ground_normal = trace.plane.normal;

		float angle = math::rad2deg(acos(velocity_backup.normalized().dot(trace.plane.normal)));
		bool is_ground_too_steep = (angle > 35);
		bool is_moving_too_fast = (velocity_backup.z < -edge_bug_vel);
		bool is_moving_down_at_correct_speed = (round(g::local->velocity().z) == -round(edge_bug_vel));
		float edgebug_velocity = sqrt(2.0f * gravity->get_float() * (player_origin.z - ground_origin.z));
		bool is_edgebug_velocity_too_high = (edgebug_velocity > edge_bug_vel);

		if (should_edge_bug && !is_ground_too_steep && is_moving_too_fast && is_moving_down_at_correct_speed && !is_edgebug_velocity_too_high) {
			float time_to_ground = sqrt(2.0f * (player_origin.z - ground_origin.z));
			static convar* gravity = interfaces::console->get_convar(("sv_gravity"));
			float edge_bug_vel = (gravity->get_float() * 0.5f * interfaces::globals->interval_per_tick);

			vec3_t player_origin = g::local->origin();
			trace_t trace;
			ray_t ray;
			trace_world_only filter;
			filter.p_skip1 = g::local;
			vec3_t start = vec3_t(player_origin.x, player_origin.y, player_origin.z + 1);
			vec3_t end = vec3_t(player_origin.x, player_origin.y, player_origin.z - 100);
			ray.initialize(start, end);
			interfaces::trace_ray->trace_ray(ray, CONTENTS_PLAYERCLIP, &filter, &trace);
			vec3_t ground_origin = trace.end;

			float angle = math::rad2deg(acos(velocity_backup.normalized().dot(trace.plane.normal)));
			float height_diff = abs(player_origin.z - ground_origin.z);
			bool is_ground_too_steep = (angle > 35);
			bool is_moving_down = (velocity_backup.z < -edge_bug_vel);
			bool has_correct_velocity = (round(g::local->velocity().z) == -round(edge_bug_vel));
			bool should_edgebug = (should_edge_bug && is_moving_down && has_correct_velocity && !is_ground_too_steep);

			if (should_edgebug && should_edge_bug) {
				vec3_t new_origin = player_origin + velocity_backup.normalized() * (2 * edge_bug_vel);
				g::local->set_abs_origin(new_origin);

				// set the velocity of the player to the edgebug velocity
				vec3_t edgebug_velocity = velocity_backup;
				edgebug_velocity.z = -edge_bug_vel;
				g::local->velocity() = edgebug_velocity;
			}
		}
	}

	return (velocity_backup.z < -edge_bug_vel && round(g::local->velocity().z) == -round(edge_bug_vel) && g::local->move_type() != movetype_ladder);
}

void features::movement::edge_bug_pre_pred( c_usercmd* cmd ) {
	if (!g::local) 
		return;

	flags_backup = g::local->flags();
	velocity_backup = g::local->velocity();

	if (should_edge_bug && should_duck_next) cmd->buttons |= in_duck;
}

void features::movement::edge_bug_post_pred( c_usercmd* cmd ) {
	if (!c::movement::edge_bug || !menu::checkkey(c::movement::edge_bug_key, c::movement::edge_bug_key_s) || !g::local) {
		should_edge_bug = false;
		return;
	}

	struct movement_vars_t {
		vec3_t viewangles;
		vec3_t view_delta;
		float forwardmove;
		float sidemove;
		int buttons;
	};

	static movement_vars_t bmove;
	movement_vars_t omove;
	omove.viewangles = cmd->view_angles;
	omove.view_delta = (cmd->view_angles - old_viewangles);
	omove.forwardmove = cmd->forward_move;
	omove.sidemove = cmd->side_move;
	omove.buttons = cmd->buttons;

	if (!should_edge_bug)
		bmove = omove;

	vec3_t current_angles;

	int cmds_predicted = interfaces::prediction->split->commands_predicted;
	int pred_cap = c::movement::edge_bug_ticks * 128;
	int pred_cound = 0;
	float highest_ground = 0.f;
	int search_dir = 0;
	int last_pred_ground = 0;
	int predict_amount = c::movement::edge_bug_ticks * 128;
	int step = 128; // search speed
	int range = 0; // search range for farther detection
	int edgebug_counter = 0;
	float ducked_height = 54.f; // height of the ducked state (adjust as needed)
	float standing_height = 72.f; // height of the standing state (adjust as needed)
	float current_height = standing_height; // current height of the player
	vec3_t local_angle = cmd->view_angles;
	vec3_t prev_origin = g::local->origin();
	interfaces::engine->get_view_angles(local_angle);
	auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(search_dir));
	vec3_t forward = entity->origin() - g::local->origin();

	for (int r = -range; r <= range; r += step) {

		for (int t = 0; pred_cound < pred_cap; t++) {
			prediction::restore_ent_to_predicted_frame(cmds_predicted - 1);
			velocity_backup = g::local->velocity();

			static int last_type = 0;

			if (should_edge_bug)
				t = last_type;

			bool do_strafe = t < 2 || t > 3;
			bool do_duck = t == 1 || t == 3;

			if (t > 3) {
				if (last_pred_ground < 2)
					break;
				bmove.view_delta += (bmove.view_delta / 2) * search_dir;
			}

			cmd->view_angles = bmove.viewangles;

			for (int i = 0; i < predict_amount && pred_cound < pred_cap; i++) {
				if (do_strafe && c::movement::edge_bug_rage) {
					cmd->view_angles += bmove.view_delta;
					cmd->forward_move = bmove.forwardmove;
					cmd->side_move = bmove.sidemove;
				}
				else {
					cmd->forward_move = 0.f;
					cmd->side_move = 0.f;
				}

				if (do_duck)
					cmd->buttons |= in_duck;
				else
					cmd->buttons &= ~in_duck;

				prediction::begin(cmd);

				should_edge_bug = check_edge_bug(cmd); 
				velocity_backup = g::local->velocity();

				if (should_edge_bug)
					player_origin_on_predicted_eb = g::local->origin();

				prediction::end();
				pred_cound++;
				edgebug_counter++;

				if (!should_edge_bug && t > 3 && g::local->origin().z < highest_ground) {
					search_dir = -1;
					break;
				}

				if (g::local->flags() & fl_onground) {
					if (t == 0)
						highest_ground = g::local->origin().z;
					if (t == 2)
						search_dir = g::local->origin().z < highest_ground ? -1 : 1;
					if (t > 3) {
						search_dir = 1;
						if (g::local->origin().z < highest_ground)
							search_dir = -1;
						else
							highest_ground = g::local->origin().z;
					}
					last_pred_ground = i;
					break;
				}

				if (g::local->move_type() == movetype_ladder)
					break;

				if (should_edge_bug) {
					if (t < 4)
						last_type = t;
					else
						last_type = 0;

					should_duck_next = do_duck;
					should_duck_next = false;

					if (!should_duck_next && g::local->origin().z - highest_ground > 24.f) {
						current_height = current_height - ducked_height;
						edgebug_counter++;
						should_duck_next = true;
					}

					if (!should_duck_next && should_edge_bug) {
						edgebug_counter++;
						should_duck_next = true;

						if (!should_duck_next && edgebug_counter % 2 == 0) {
							current_height = current_height - ducked_height;
							edgebug_counter++;
							should_duck_next = true;
						}
					}
					if (do_strafe && c::movement::edge_bug_rage) {
						cmd->view_angles = bmove.viewangles + bmove.view_delta;
						bmove.viewangles = cmd->view_angles;
						if (c::movement::edge_bug_rage)
							interfaces::engine->set_view_angles(cmd->view_angles);
					}
					if (i == 1) {
						edgebugs++;

						last_edgebug = interfaces::globals->cur_time;
						//effect_alpha = edgebug_effects::effect_time;    
						player_origin_on_eb = g::local->origin();
						eb_origins.push_back(g::local->origin());

						if (c::movement::edge_bug_detection_printf) {
							std::stringstream ss;

							ss << "\x08" << " \x08"
								<< "%c"
								<< "delusional"
								<< "\x08" << "\x08 | edgebugged";

							interfaces::chat_element->chatprintf(0, 0, ss.str().c_str(), menu::color_p);
						}

						switch (c::movement::edge_bug_detection_sound) {
						case 0:
							break;
						case 1:
							interfaces::surface->play_sound(xs("survival/money_collect_05.wav"));
							break;
						case 2:
							interfaces::surface->play_sound(xs("buttons/arena_switch_press_02.wav"));
							break;
						}

						detected_normal_edge_bug = true;
					}
					else
						detected_normal_edge_bug = false;

					return;
				}
			}
		}
	}
	cmd->view_angles = omove.viewangles;
	cmd->forward_move = omove.forwardmove;
	cmd->side_move = omove.sidemove;
	cmd->buttons = omove.buttons;
}

bool hb = false;
void features::movement::auto_align(c_usercmd* cmd) {
	if (!c::movement::auto_align) {
		return;
	}

	if (!g::local || !g::local->is_alive() || g::local->flags() & fl_onground) {
		should_ps_standing = false;
		return;
	}

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip) {
		should_ps_standing = false;
		return;
	}

	float max_radias = m_pi * 2.f;
	float step = max_radias / 16.f;
	vec3_t start_pos = g::local->abs_origin();
	const auto mins = g::local->collideable()->mins();
	const auto maxs = g::local->collideable()->maxs();
	trace_world_only fil;

	for (float a = 0.f; a < max_radias; a += step) {
		vec3_t end_pos;
		end_pos.x = cos(a) + start_pos.x;
		end_pos.y = sin(a) + start_pos.y;
		end_pos.z = start_pos.z;
		trace_t trace;
		ray_t ray;
		ray.initialize(start_pos, end_pos, mins, maxs);

		// traceraying the whole circle around us trying to find a wall
		interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &fil, &trace);
		if ((trace.flFraction < 1.f) && (trace.plane.normal.z == 0.f)) {
			// found a wall
			vec3_t angles{ trace.plane.normal.x * -0.005f, trace.plane.normal.y * -0.005f, 0.f };
			const vec3_t end_pos2 = start_pos + angles;
			trace_t trace2;
			// traceraying towards the wall we found to see if we're aligned or not
			ray_t ray2;
			ray2.initialize(start_pos, end_pos2, mins, maxs);
			interfaces::trace_ray->trace_ray(ray2, MASK_PLAYERSOLID, &fil, &trace2);
			if (trace2.flFraction == 1.f) {
				vec3_t to_wall = angles.to_angle();
				to_wall.normalize();
				vec3_t velo = g::local->velocity();
				velo.z = 0.f;
				// checking if your velocity if not zero
				if (velo.length_2d() > 0.f) {
					vec3_t velo_ang = velo.to_angle();
					vec3_t delta = velo_ang - to_wall;
					delta.normalize();
					// comparing our velocity with a vector towards the wall to see if we actually want to align
					if (fabsf(delta.z) > 90.f)
						break;
				}
				// calculating the direction towards the wall to forward and sidemove
				float rotation = math::deg2rad(to_wall.y - cmd->view_angles.y);
				float cos_rot = cos(rotation);
				float sin_rot = sin(rotation);
				should_align = false;

				for (int i = 1; i <= 5; i++) {
					// try different forward and sidemove variations to find one that alignes in 1 tick
					float forwardmove = cos_rot * i * 9;
					float sidemove = -sin_rot * i * 9;
					c_usercmd fakecmd = *cmd;
					fakecmd.forward_move = forwardmove;
					fakecmd.side_move = sidemove;
					prediction::begin(&fakecmd);
					prediction::end();

					vec3_t start_pos2 = g::local->abs_origin();
					const vec3_t end_pos3 = start_pos2 + angles;
					trace_t trace3;
					ray_t ray3;
					ray3.initialize(start_pos2, end_pos3, mins, maxs);
					interfaces::trace_ray->trace_ray(ray3, MASK_PLAYERSOLID, &fil, &trace3);
					if (trace3.flFraction < 1.f) {
						cmd->forward_move = forwardmove;
						cmd->side_move = sidemove;
						should_align = true;
						prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
						break;
					}
					prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
				}
				if (!should_align) {
					for (int i = 1; i <= 5; i++) {
						// same shit here but in 2 ticks bc it might not be able to align in one
						float forwardmove = cos_rot * i * 9;
						float sidemove = -sin_rot * i * 9;
						vec3_t absorigin = g::local->abs_origin();
						c_usercmd fakecmd2 = *cmd;
						fakecmd2.forward_move = forwardmove;
						fakecmd2.side_move = sidemove;
						prediction::begin(&fakecmd2);
						prediction::end();
						prediction::begin(&fakecmd2);
						prediction::end();

						vec3_t start_pos2 = g::local->abs_origin();
						start_pos2.z = absorigin.z;
						const vec3_t end_pos3 = start_pos2 + angles;
						trace_t trace3;
						ray_t ray4;
						ray4.initialize(start_pos2, end_pos3, mins, maxs);
						interfaces::trace_ray->trace_ray(ray4, MASK_PLAYERSOLID, &fil, &trace3);
						if (trace3.flFraction < 1.f) {
							cmd->forward_move = forwardmove;
							cmd->side_move = sidemove;
							should_align = true;
							prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
							break;
						}
						prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
					}
				}
			}
			else {
				// this part of code works if we are already aligned
				for (int i = 45.f; i > 0.f; i -= 5.f) {
					// finding good forward and sidemove values to keep us stable on the surf
					float forwardmove = 0.f;
					float sidemove = 0.f;
					// direct it towards the direction we're pressing
					if (cmd->buttons & in_forward)
						forwardmove = i;
					if (cmd->buttons & in_back)
						forwardmove = -i;
					if (cmd->buttons & in_moveleft)
						sidemove = -i;
					if (cmd->buttons & in_moveright)
						sidemove = i;

					if (!forwardmove && !sidemove && c::movement::freelook_surf
						&& menu::checkkey(c::movement::pixel_surf_key, c::movement::pixel_surf_key_s)) {
						// OR direct it towards the wall when we're not pressing anything and freelook is enabled
						vec3_t to_wall = angles.to_angle();
						to_wall.normalize();
						float rotation = math::deg2rad(to_wall.y - cmd->view_angles.y);
						float cos_rot = cos(rotation);
						float sin_rot = sin(rotation);
						forwardmove = cos_rot * i;
						sidemove = -sin_rot * i;
					}

					// checking if the value is good to keep stable and using it
					c_usercmd fakecmd5 = *cmd;
					fakecmd5.forward_move = forwardmove;
					fakecmd5.side_move = sidemove;
					prediction::begin(&fakecmd5);
					prediction::end();

					float zvelo = g::local->velocity().z;
					if (zvelo == -6.25f) {
						prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
						cmd->forward_move = forwardmove;
						cmd->side_move = sidemove;

						if (!hb)
							should_ps_standing = true;

						break;
					}
					else {
						should_ps_standing = false;
					}
					prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
				}
			}
			break;
		}
	}
}

int saved_buttons;
int predictedtickamount = -1;
void features::movement::pixel_surf(c_usercmd* cmd) {
	if (!c::movement::pixel_surf || !menu::checkkey(c::movement::pixel_surf_key, c::movement::pixel_surf_key_s))
		return;

	if (!g::local || !g::local->is_alive() || g::local->flags() & fl_onground) {
		should_ps_crouch = false;
		return;
	}

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip) {
		should_ps_crouch = false;
		return;
	}

	if (should_edge_bug)
		return;

	int tickspredicted = 0;
	bool foundpx = false;
	int restorebut = cmd->buttons;

	prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);

	for (int tickspredicted = 0; tickspredicted < 3; tickspredicted++) {
		cmd->buttons |= in_duck;
		float pre_pred_vel = g::local->velocity().z;

		prediction::begin(cmd);

		float post_pred_vel = g::local->velocity().z;
		int post_pred_flags = g::local->flags();

		if (pre_pred_vel < -6.25f && post_pred_vel < -6.25f && post_pred_vel > pre_pred_vel) {
			foundpx = true;
			predictedtickamount = tickspredicted;
		}

		cmd->buttons = restorebut;

		prediction::end();

		if (g::local->move_type() == movetype_ladder || (post_pred_flags & fl_onground) || post_pred_vel > 0.0) {
			hb = true;
			break;
		}
		else
			hb = false;
	}

	if (foundpx) {
		should_ps_crouch = true;
		restorebut |= in_duck;
	}
	else if (g::local->velocity().z == -6.25f || g::local->velocity().z == -6.f || g::local->velocity().z == -3.125f && fl_onground) {
		should_ps_crouch = true;
		cmd->buttons |= in_duck;
	}
	else {
		should_ps_crouch = false;
	}

	if (predictedtickamount == -1) {

	}
	else {
		--predictedtickamount;
		cmd->buttons = saved_buttons;
	}

	prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
}

void features::movement::pixel_surf_fix(c_usercmd* cmd) {
	if (!c::movement::pixel_surf_fix) {
		return;
	}

	if (!g::local->is_alive())
		return;

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip) {
		return;
	}

	if (cmd->buttons & in_duck)
		return;

	vec3_t velo = g::local->velocity();

	if ((velo.length_2d() <= 285.92f) || (velo.z >= 0.f))
		return;

	prediction::begin(cmd);
	prediction::end();

	if (g::local->flags() & fl_onground) {
		int tickrate = 1 / interfaces::globals->interval_per_tick;
		float airaccelerate = interfaces::console->get_convar("sv_airaccelerate")->get_float();
		float wishdelta = (velo.length_2d() - 285.92f) * tickrate / airaccelerate;
		velo = velo * -1.f;
		vec3_t velo_ang = velo.to_angle();
		velo_ang.normalize();
		float rotation = math::deg2rad(velo_ang.y - cmd->view_angles.y);
		float cos_rot = cos(rotation);
		float sin_rot = sin(rotation);
		cmd->forward_move = cos_rot * wishdelta;
		cmd->side_move = -sin_rot * wishdelta;
	}
	prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
}

void features::movement::auto_duck(c_usercmd* cmd) {
	if (!menu::checkkey(c::movement::auto_duck_key, c::movement::auto_duck_key_s))
		return;

}

void features::movement::fake_backwards(c_usercmd* cmd) {
	if (!c::movement::fakebackwards) {
		return;
	}

	if (!interfaces::engine->is_in_game() || !g::local->is_alive()) 
		return;



}

auto interpolate(const color_t first_color, const color_t& second_color, const float multiplier) {
	return color_t(first_color.r() + std::clamp(multiplier, 0.f, 1.f) * (second_color.r() - first_color.r()),
		first_color.g() + std::clamp(multiplier, 0.f, 1.f) * (second_color.g() - first_color.g()),
		first_color.b() + std::clamp(multiplier, 0.f, 1.f) * (second_color.b() - first_color.b()),
		first_color.a() + std::clamp(multiplier, 0.f, 1.f) * (second_color.a() - first_color.a()));
}

void features::movement::velocity_indicator( ) {
	if (!g::local || !g::local->is_alive() || !interfaces::engine->is_in_game()) {
		return;
	}

	if (c::movement::velocity_indicator) {
		int x, y;
		interfaces::engine->get_screen_size(x, y);
		y -= c::movement::velocity_indicator_position;

		static bool on_ground = false;
		static auto take_off = 0;
		static auto take_off_time = 0.0f;
		static auto last_on_ground = false;

		int vel = round(g::local->velocity().length_2d());
		on_ground = g::local->flags() & fl_onground;

		if (last_on_ground && !on_ground) {
			take_off = vel;
			take_off_time = interfaces::globals->cur_time + (c::movement::velocity_indicator_disable_ong_show_pre ? 0.0f : 3.5f);
		}

		last_on_ground = on_ground;

		static float velocity_alpha = 0.0f;
		velocity_alpha = 1.f * (float(round(vel)) / 255.f);

		auto velocity_color = [](int velocity) -> color_t {
			if (velocity > 0)
				return color_t(c::movement::velocity_indicator_positive_clr[0], c::movement::velocity_indicator_positive_clr[1], c::movement::velocity_indicator_positive_clr[2], 255.f);
			else if (velocity < 0)
				return color_t(c::movement::velocity_indicator_negative_clr[0], c::movement::velocity_indicator_negative_clr[1], c::movement::velocity_indicator_negative_clr[2],255.f);
			else if (velocity > -1 && velocity < 1)
				return color_t(c::movement::velocity_indicator_neutral_clr[0], c::movement::velocity_indicator_neutral_clr[1], c::movement::velocity_indicator_neutral_clr[2], 255.f);
		};

		color_t velocity_clr_int = color_t(c::movement::velocity_indicator_fade_clr3[0], c::movement::velocity_indicator_fade_clr3[1], c::movement::velocity_indicator_fade_clr3[2], c::movement::velocity_indicator_fade_clr3[3]);
		color_t velocity_clr_custom = color_t(c::movement::velocity_indicator_custom_clr[0], c::movement::velocity_indicator_custom_clr[1], c::movement::velocity_indicator_custom_clr[2], c::movement::velocity_indicator_custom_clr[3]);

		int tick_count = interfaces::globals->tick_count;
		last_delta = vel - last_vel;

		const auto should_draw_takeoff =
			(!on_ground || (take_off_time > interfaces::globals->cur_time)) && c::movement::velocity_indicator_show_pre;

		std::string place_holder = "";

		if (should_draw_takeoff)
			place_holder = std::to_string(vel) + " (" + std::to_string(take_off) + ")";
		else
			place_holder = std::to_string(vel);

		im_render.text(x / 2, y, c::fonts::indi_size, fonts::indicator_font, place_holder.c_str(), true, c::movement::velocity_indicator_custom_color ? interpolate(velocity_clr_int, velocity_clr_custom, velocity_alpha) : velocity_color(last_delta), c::fonts::indi_font_flag[9], c::fonts::indi_font_flag[10]);

		if (fabs(tick_count - last_tick) > 5) {
			last_tick = tick_count;
			last_vel = vel;
		}
	}
}

void features::movement::stamina_indicator( ) {
	if (!g::local || !g::local->is_alive() || !interfaces::engine->is_in_game()) {
		return;
	}

	if (c::movement::stamina_indicator) {
		int x, y;
		interfaces::engine->get_screen_size(x, y);
		y -= c::movement::stamina_indicator_position;

		static float last_speed = 0, last_jump_speed = 0;
		static float last_vel_update = 0.0f;
		static bool last_onground = false;
		static auto take_off_time = 0.0f;
		bool current_onground = g::local->flags() & fl_onground;
		float stamina = g::local->stamina();
		float current_speed = stamina;

		if (last_onground && !current_onground) {
			last_jump_speed = current_speed;
			take_off_time = interfaces::globals->cur_time + (c::movement::stamina_indicator_disable_ong_show_pre ? 0.0f : 3.5f);
		}

		static float stamina_alpha = 0.f;
		stamina_alpha = 1.f * (float(stamina) / 25.f);

		color_t stamina_clr = current_speed < last_speed ? color_t(c::movement::stamina_indicator_clr[0], c::movement::stamina_indicator_clr[1], c::movement::stamina_indicator_clr[2], c::movement::stamina_indicator_clr[3]) : color_t(c::movement::stamina_indicator_clr[0], c::movement::stamina_indicator_clr[1], c::movement::stamina_indicator_clr[2], c::movement::stamina_indicator_clr[3]);
		color_t stamina_clr_int = current_speed < last_speed ? color_t(c::movement::stamina_fade_clr[0], c::movement::stamina_fade_clr[1], c::movement::stamina_fade_clr[2], c::movement::stamina_fade_clr[3]) : color_t(c::movement::stamina_fade_clr[0], c::movement::stamina_fade_clr[1], c::movement::stamina_fade_clr[2], c::movement::stamina_fade_clr[3]);

		std::string value_str;

		std::stringstream ss;
		ss << std::fixed << std::setprecision(current_speed > 0.0f ? 1 : 1) << current_speed;
		value_str = ss.str();

		std::string str = value_str;

		std::stringstream ss1;
		ss1 << std::fixed << std::setprecision(1) << last_jump_speed;
		std::string value_str2 = ss1.str();

		const auto should_draw_takeoff =
			(!current_onground || (take_off_time > interfaces::globals->cur_time)) && c::movement::stamina_indicator_show_pre;

		if (should_draw_takeoff)
			str += " (" + value_str2 + ")";
		else
			str = value_str;

		im_render.text(x / 2, y, c::fonts::indi_size, fonts::indicator_font, str.c_str(), true, c::movement::stamina_indicator_fade ? interpolate(stamina_clr_int, stamina_clr, stamina_alpha) : stamina_clr, c::fonts::indi_font_flag[9], c::fonts::indi_font_flag[10]);


		if (interfaces::globals->cur_time > last_vel_update) {
			last_speed = current_speed;
			last_vel_update = interfaces::globals->cur_time + 0.05f;
		}
		last_onground = current_onground;
	}
}

void render_indicator(int key, int key_s, int& alpha, color_t& clr, const char* text, bool tick_count, int should_detect, bool allow_detection_clr, int& position, int saved_tick = 0) {
	int x, y;
	interfaces::engine->get_screen_size(x, y);
	y -= c::movement::indicators_position;

    color_t default_clr(c::movement::velocity_indicator_custom_clr2[0], c::movement::velocity_indicator_custom_clr2[1], c::movement::velocity_indicator_custom_clr2[2]);

	if (allow_detection_clr) {
		if (tick_count)
			clr = interfaces::globals->tick_count - saved_tick < c::movement::detection_saved_tick ? color_t(c::movement::indicator_detect_clr[0], c::movement::indicator_detect_clr[1], c::movement::indicator_detect_clr[2]) : default_clr;
		else
			clr = should_detect ? color_t(c::movement::indicator_detect_clr[0], c::movement::indicator_detect_clr[1], c::movement::indicator_detect_clr[2]) : default_clr;
	}
	else
		clr = default_clr;

	float fading_speed = (c::movement::indicators_fading_speed*4.f)/100.f;

	if (menu::checkkey(key, key_s)) {
		alpha = std::clamp(alpha + fading_speed, 0.f, 255.f);
		if (!c::movement::indicators_allow_animation) {
			im_render.text(x / 2, y + position, c::fonts::sub_indi_size, fonts::sub_indicator_font, text, true, clr, c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
			position += (24 + c::movement::indicators_gap);
		}
	}
	else {
		alpha = std::clamp(alpha - fading_speed, 0.f, 255.f);
		if (tick_count)
			should_detect = false;
	}

	if (c::movement::indicators_allow_animation && alpha >= 1) {
		im_render.text(x / 2, y + position, c::fonts::sub_indi_size, fonts::sub_indicator_font, text, true, clr.get_color_edit_alpha_int(alpha), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		position += (24 + c::movement::indicators_gap);
	}
}

void features::movement::indicators() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game() || !g::local || !g::local->is_alive()) {
		saved_tick_jb = 0;
		saved_tick_lj = 0;
		saved_tick_ej = 0;
		saved_tick_mj = 0;
		return;
	}

	if (detected_normal_jump_bug)
		saved_tick_jb = interfaces::globals->tick_count;

	if (should_ej)
		saved_tick_ej = interfaces::globals->tick_count;

	if (should_lj)
		saved_tick_lj = interfaces::globals->tick_count;

	if (should_mj)
		saved_tick_mj = interfaces::globals->tick_count;

	color_t ps_clr, al_clr, sh_clr, eb_clr, jb_clr, ej_clr, lj_clr, mj_clr, lg_clr, as_clr;
	static int p_alpha, al_alpha, sh_alpha, eb_alpha, jb_alpha, ej_alpha, lj_alpha, mj_alpha, lb_alpha, as_alpha = 0;
	int position = 0;
	bool is_ps = should_ps_crouch || should_ps_standing;

	if (c::movement::indicators_show[0])
		render_indicator(c::movement::edge_bug_key, c::movement::edge_bug_key_s, eb_alpha, eb_clr, "eb", false, should_edge_bug, c::movement::detection_clr_for[0], position);
	
	if (c::movement::indicators_show[2])
		render_indicator(c::movement::long_jump_key, c::movement::long_jump_key_s, lj_alpha, lj_clr, "lj", true, should_lj, c::movement::detection_clr_for[2], position, saved_tick_lj);

	if (c::movement::indicators_show[5])
		render_indicator(c::movement::edge_jump_key, c::movement::edge_jump_key_s, ej_alpha, ej_clr, "ej", true, should_ej, c::movement::detection_clr_for[6], position, saved_tick_ej);

	if (c::movement::indicators_show[3])
		render_indicator(c::movement::mini_jump_key, c::movement::mini_jump_key_s, mj_alpha, mj_clr, "mj", true, should_mj, c::movement::detection_clr_for[3], position, saved_tick_mj);

	if (c::movement::indicators_show[4])
		render_indicator(c::movement::pixel_surf_key, c::movement::pixel_surf_key_s, p_alpha, ps_clr, "ps", false, is_ps, c::movement::detection_clr_for[4], position);
	
	if (c::movement::indicators_show[1])
		render_indicator(c::movement::jump_bug_key, c::movement::jump_bug_key_s, jb_alpha, jb_clr, "jb", true, detected_normal_jump_bug, c::movement::detection_clr_for[1], position, saved_tick_jb);
	
	if (c::movement::indicators_show[6])
		render_indicator(c::movement::ladder_bug_key, c::movement::ladder_bug_key_s, lb_alpha, lg_clr, "lb", false, should_lb, c::movement::detection_clr_for[6], position);

	if (c::movement::indicators_show[7])
		render_indicator(c::movement::auto_strafe_key, c::movement::auto_strafe_key_s, as_alpha, as_clr, "autostrafing", false, false, c::movement::detection_clr_for[7], position);
}

features::movement::velocity_data_t current_vel_data;
static std::deque<std::pair<float, bool>> stamina_data;
void features::movement::gather_vel_graph_data( ) {
	if (!g::local || !g::local->is_alive() || !c::movement::velocity_graph)
		return;

	if (!interfaces::engine->is_connected())
		return;

	float speed = g::local->velocity().length_2d();
	int flags = g::local->flags();
	bool on_ground = flags & fl_onground;

	current_vel_data.speed = speed;
	current_vel_data.on_ground = on_ground;
	current_vel_data.edge_bug = detected_normal_edge_bug;
	current_vel_data.jump_bug = detected_normal_jump_bug;

	velocity_data.insert(velocity_data.begin(), current_vel_data);

	if (velocity_data.size() > 185)
		velocity_data.pop_back();
}

void features::movement::gather_stam_graph_data( ) {
	if (!g::local || !g::local->is_alive() || !c::movement::stamina_graph)
		return;

	if (!interfaces::engine->is_connected())
		return;

	stamina_data.push_front({g::local->stamina(), !(g::local->flags() & fl_onground) });

	if (stamina_data.size() > 185)
		stamina_data.pop_back();
}

void features::movement::velocity_graph_draw( ) {
	if (!g::local->is_alive() || !g::local)
		return;

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) {
		velocity_data.clear();
		return;
	}

	if (c::movement::velocity_graph) {
		int x, y;
		interfaces::engine->get_screen_size(x, y);

		if (velocity_data.size() == 185) {
			for (auto i = 0; i < velocity_data.size() - 2; i++) {
				auto& curdata = velocity_data[i];//
				auto& nextdata = velocity_data[i + 1];
				auto cur = curdata.speed;
				auto next = nextdata.speed;
				auto landed = !curdata.on_ground && nextdata.on_ground;
				auto eb = curdata.edge_bug;
				auto jb = curdata.jump_bug;
				auto ps = curdata.pixel_surf;
				int alpha = 255;

				switch (c::movement::graph_fade) {
				case 0:
					//no faded
					break;
				case 1:
					//faded start and end
					if (i < 70) {
						float a = ((i + 1) / 70.f) * 255.f;
						alpha = (int)a;
					}
					else if (i > velocity_data.size() - 71) {
						int todivide = velocity_data.size() - i;
						float a = (todivide / 70.f) * 255.f;
						alpha = (int)a;
					}
					break;
				case 2:
					//faded by vel
					if (i < 65) {
						float a = ((i + 1) / 65.f) * 255.f;
						alpha = (int)a;
					}
					else if (i > velocity_data.size() - 66) {
						int todivide = velocity_data.size() - i;
						float a = (todivide / 65.f) * 255.f;
						alpha = (int)a;
					}

					alpha *= std::clamp(((cur + next) / 2) / c::movement::velocity_tofade, 0.f, 1.f);
					break;
				case 3:
					//faded end
					if (i > velocity_data.size() - 66) {
						int todivide = velocity_data.size() - i;
						float a = (todivide / 65.f) * 255.f;
						alpha = (int)a;
					}
					break;
				case 4:
					//faded start
					if (i < 65) {
						float a = ((i + 1) / 65.f) * 255.f;
						alpha = (int)a;
					}
					break;
				}

				alpha = std::clamp(alpha, 0, 255);

				color_t linecolor = color_t(c::movement::velocity_graph_color[0], c::movement::velocity_graph_color[1], c::movement::velocity_graph_color[2], (alpha / 255.f));

				im_render.drawline(
					((x / 2) + (((velocity_data.size() / 2) * c::movement::velocity_linewidth))) - ((i - 1) * c::movement::velocity_linewidth),
					y * c::movement::graph_xscreenpos - 15 - ((std::clamp(cur, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height),
					x / 2 + (((velocity_data.size() / 2) * c::movement::velocity_linewidth)) - i * c::movement::velocity_linewidth,
					y * c::movement::graph_xscreenpos - 15 - ((std::clamp(next, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height),
					linecolor, 0);

				if (landed && c::movement::velocity_graph_show_landed_speed && !eb)
					im_render.text(((x / 2) + (((velocity_data.size() / 2) * c::movement::velocity_linewidth))) - ((i + 1) * c::movement::velocity_linewidth), y * c::movement::graph_xscreenpos - 30 - ((std::clamp(next, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height), 11, fonts::esp_misc, std::to_string((int)round(next)).c_str(), true, color_t(1.f, 1.f, 1.f, (alpha / 255.f)), true);

				if (eb && c::movement::velocity_graph_show_edge_bug)
					im_render.text(((x / 2) + (((velocity_data.size() / 2) * c::movement::velocity_linewidth))) - ((i + 1) * c::movement::velocity_linewidth), y * c::movement::graph_xscreenpos - 10 - ((std::clamp(next, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height), 11, fonts::esp_misc, "eb", true, color_t(1.f, 1.f, 1.f, (alpha / 255.f)), true);

				if (jb  && c::movement::velocity_graph_show_jump_bug)
					im_render.text(((x / 2) + (((velocity_data.size() / 2) * c::movement::velocity_linewidth))) - ((i + 1) * c::movement::velocity_linewidth), y * c::movement::graph_xscreenpos - 10 - ((std::clamp(next, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height), 11, fonts::esp_misc, "jb", true, color_t(1.f, 1.f, 1.f, (alpha / 255.f)), true);
			}
		}
	}
}

void features::movement::stamina_graph_draw( ) {
	if (!g::local->is_alive() || !g::local)
		return;

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) {
		stamina_data.clear();
		return;
	}

	if (c::movement::stamina_graph) {
		int x, y;
		interfaces::engine->get_screen_size(x, y);

		if (stamina_data.size() == 185) {
			for (auto i = 0; i < stamina_data.size() - 2; i++) {
				auto& curdata = stamina_data[i];//
				auto& nextdata = stamina_data[i + 1];
				auto cur = curdata.first;
				auto next = nextdata.first;
				auto landed = curdata.second && !nextdata.second;
				int alpha = 255;

				switch (c::movement::graph_fade) {
				case 0:
					//no faded
					break;
				case 1:
					//faded start and end
					if (i < 65) {
						float a = ((i + 1) / 65.f) * 255.f;
						alpha = (int)a;
					}
					else if (i > stamina_data.size() - 66) {
						int todivide = stamina_data.size() - i;
						float a = (todivide / 65.f) * 255.f;
						alpha = (int)a;
					}
					break;
				case 2:
					//faded by vel
					if (i < 65) {
						float a = ((i + 1) / 65.f) * 255.f;
						alpha = (int)a;
					}
					else if (i > stamina_data.size() - 66) {
						int todivide = stamina_data.size() - i;
						float a = (todivide / 65.f) * 255.f;
						alpha = (int)a;
					}

					alpha *= std::clamp(((cur + next) / 2) / 1.f, 0.f, 1.f);//ee
					break;
				case 3:
					//faded end
					if (i > velocity_data.size() - 66) {
						int todivide = velocity_data.size() - i;
						float a = (todivide / 65.f) * 255.f;
						alpha = (int)a;
					}
					break;
				case 4:
					//faded start
					if (i < 65) {
						float a = ((i + 1) / 65.f) * 255.f;
						alpha = (int)a;
					}
					break;
				}

				alpha = std::clamp(alpha, 0, 255);

				color_t linecolor = color_t(c::movement::stamina_graph_color[0], c::movement::stamina_graph_color[1], c::movement::stamina_graph_color[2], (alpha / 255.f));

				im_render.drawline(
					((x / 2) + (((velocity_data.size() / 2) * c::movement::velocity_linewidth))) - ((i - 1) * c::movement::velocity_linewidth),
					y * c::movement::graph_xscreenpos - 15 - ((std::clamp(cur, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height),
					x / 2 + (((velocity_data.size() / 2) * c::movement::velocity_linewidth)) - i * c::movement::velocity_linewidth,
					y * c::movement::graph_xscreenpos - 15 - ((std::clamp(next, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height),
					linecolor, 0);

			}
		}
	}
}