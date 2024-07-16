#pragma once
#include "../../sdk/sdk.hpp"

namespace features::misc {
	inline int spec_p = 0;
	struct notify_t {
		std::string text;
		float time;
		color _color;
		notify_t( std::string _text, color __color ) {
			text = _text;
			_color = __color;
			time = interfaces::globals->cur_time;
		}
	};
	inline std::vector<notify_t> notify_list;

	void draw();
	void notify( std::string text, color _color );
	void spectators_list();
	void kz_practice();
	void kz_practice_logic(c_usercmd* cmd);
	void reveal_server_ranks(c_usercmd* cmd);
	void door_spam(c_usercmd* cmd);
	void fix_mouse_delta(c_usercmd* cmd);
	void reset_thirdperson();
	void thirdperson();
	void view_model();
	void clantag_spammer();
	void grenade_trail();
	void force_crosshair();
	void sniper_crosshair();
	void radaringame(player_t* p);
	void vote_revealer(i_game_event* event);
	void hit_info(i_game_event* event);
	void kill_say(i_game_event* event);

	namespace hitmarker {
		void draw();
		void event(i_game_event* event);
	}

	namespace jumpstats {
		void jumpstats(c_usercmd* cmd);
		void resetjumpstats();
		void gotjump();
	}
}