#include "../hooks.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/skins/skins.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/misc/misc.hpp"

void __stdcall sdk::hooks::frame_stage_notify::frame_stage_notify( int stage ) {
	player_t* local_player = player_t::get_local_player();

	if (stage == frame_render_start) {
		features::misc::force_crosshair();
		features::visuals::flashalpha();
		features::visuals::skybox_changer();
		features::visuals::nosmoke();
		features::weather::update_weather();
		features::visuals::apply_zoom();
		backtrack.setup_records();
		features::visuals::console();
	}
	else if (stage == frame_render_end) {
		features::visuals::fog();
	}
	else if (stage == frame_net_update_postdataupdate_start) {
		features::skins::agent_changer( );
		features::skins::knife_changer( );
		features::skins::gloves_changer();
		features::skins::full_update(local_player);
	}
	else if (stage == frame_net_update_end) {

	}

	if (!interfaces::engine->is_in_game()) {
		fakeping.clear_sequence();
		return ofunc(interfaces::client, stage);
	}

	ofunc( interfaces::client, stage );
}