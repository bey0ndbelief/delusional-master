#pragma once
#include "../../sdk/sdk.hpp"
#include <filesystem>
#include <cstddef>
#include <cstdio>

namespace features::movement {
	inline std::vector<vec3_t> eb_origins;
	inline vec3_t velocity_backup;
	inline vec3_t old_viewangles;
	inline vec3_t player_origin_on_predicted_eb;
	inline vec3_t player_origin_on_eb;
	inline bool should_duck_next;
	inline bool should_edge_bug;
	inline bool detected_normal_edge_bug;
	inline bool detected_ladder_glide;
	inline bool detected_normal_jump_bug = false;
	inline bool should_ps_standing;
	inline bool should_ps_crouch;
	inline bool should_align;
	inline bool should_lj;
	inline bool should_ej;
	inline bool should_mj;
	inline bool should_lb;
	inline bool do_long_jump = false;
	inline bool do_mini_jump = false;
	inline bool did_jump_last = false;
	inline int last_tick = 0;
	inline int last_delta = 0;
	inline int last_vel = 0;
	inline int pre = 0;
	inline int edgebugs = 0;
	inline int edgebug_combo = 0;
	inline int flags_backup;
	inline int saved_tick_jb = 0;
	inline int saved_tick_lj = 0;
	inline int saved_tick_ej = 0;
	inline int saved_tick_mj = 0;
	inline float last_update_time = 0.f;
	inline static float last_edgebug;
	inline float effect_alpha;
	inline float hiteffect_alpha;

	void bhop(c_usercmd* cmd);//
	void delay_hop(c_usercmd* cmd);
	void edge_bug_post_pred( c_usercmd* cmd );
	void edge_bug_pre_pred( c_usercmd* cmd );
	bool check_edge_bug(c_usercmd* cmd);
	void visualize_edge_bug();
	void pixel_surf(c_usercmd* cmd);
	void pixel_surf_fix(c_usercmd* cmd);
	void jump_bug(c_usercmd* cmd, int pre_flags, vec3_t pre_pred_vel);
	void movement_correction(c_usercmd* cmd, vec3_t old_angles, vec3_t wish_angle, player_t* local_player);
	void edge_jump(c_usercmd* cmd, int pre_pred_flags);
	void crouch_bug(c_usercmd* cmd, int pre_pred_flags);
	void long_jump(c_usercmd* cmd );
	void ladder_jump(c_usercmd* cmd);
	void ladder_bug(c_usercmd* cmd);
	void mini_jump(c_usercmd* cmd, int pre_pred_flags);
	void auto_duck(c_usercmd* cmd);
	void fake_backwards(c_usercmd* cmd);
	void auto_strafe(c_usercmd* cmd, vec3_t& current_angle);
	void fix_mov(c_usercmd* cmd, float yaw);
	void blockbot(c_usercmd* cmd);
	void strafe_optimizer(c_usercmd* user_cmd, int pre_flags);
	void velocity_indicator();
	void stamina_indicator();
	void indicators();
	void auto_align(c_usercmd* cmd);
	void gather_vel_graph_data( );
	void velocity_graph_draw( );
	void gather_stam_graph_data( );
	void stamina_graph_draw( );

	struct velocity_data_t {
		float speed;
		bool on_ground;
		bool edge_bug;
		bool jump_bug;
		bool pixel_surf;
	};

	inline std::vector<velocity_data_t> velocity_data;

	struct tick_info {
		tick_info() {
			velocity = g::local->velocity();
			origin = g::local->origin();
			stamina = g::local->stamina();
			in_air = !(g::local->flags() & fl_onground);
		}
		vec3_t velocity;
		vec3_t origin;
		float stamina;
		bool in_air;
	};

	inline std::deque<tick_info> move_info;
}

namespace features::calculator {
	void run();
	void draw_info();


}

class savingroute {
public:
	explicit savingroute(const char*);
	void load(size_t);
	void save(size_t, std::string name) const;
	void add(const char*);
	void remove(size_t);
	void rename(size_t, const char*);
	void listroutes();

	constexpr auto& getRoutes() {
		return routes;
	}

private:
	std::filesystem::path path;
	std::vector<std::string> routes;
};

typedef savingroute;

inline std::unique_ptr<savingroute> route;

class mrecorder {
public:
	void draw();
	void hook(c_usercmd* cmd, vec3_t unpredictpos);
	void fix_mov(c_usercmd* cmd, vec3_t& ang);
	void drawroute();
	void infowindow();
	void forcestop();
	vec3_t origin_r;
};

inline mrecorder* mrec;
