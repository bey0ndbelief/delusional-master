#pragma once
#include "../../sdk/sdk.hpp"
#include "../../hooks/hooks.hpp"

namespace features::chams {
	void run(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix);
	void run_bt(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix);
	void run_other(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix);
	void initialize();
}

struct bbox_t {
	int x, y, w, h;
	bbox_t() = default;
	bbox_t(const int x, const int y, const int w, const int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};

namespace features::visuals {
	inline bool console_vis = false;

	bool get_playerbox(player_t* entity, bbox_t& in);
	namespace player {
		void run();
		void animate(player_t* entity, const int index);
		void flags(player_t* entity, bbox_t bbox);
		void backtrack_dot(player_t* entity);
		void backtrack_skeleton(player_t* entity);
		void skeleton(player_t* entity);
		void draw_name(player_t* entity, bbox_t bbox);
		void draw_health(player_t* entity, bbox_t bbox);
		void draw_armor(player_t* entity, bbox_t bbox);
		void draw_weapon(player_t* entity, bbox_t bbox);
		void draw_box(player_t* entity, bbox_t bbox);

	}

	namespace glow {
		void run();
	}

	void trail();
	void removals();
	void oof_arrows(player_t* player);
	void motion_blur(view_setup_t* setup);
	void dropped_weapons(player_t* entity);
	void bomb_esp(entity_t* entity);
	void apply_zoom();
	void fog();
	void gravity_ragdoll();
	void skybox_changer();
	void fullbright();
	void flashalpha();
	void nosmoke();
	void dlights(player_t* entity);
	void run_freecam(c_usercmd* cmd, vec3_t angles);
	void freecam(view_setup_t* setup);
	void key_strokes();
	void console();

}

namespace features::weather {
	static bool last_state = false;
	static bool created_rain = false;

	static precipitation_t* rain_entity = nullptr;
	static c_client_class* precipitation = nullptr;

	void reset_weather(const bool cleanup = true);
	void update_weather();

}