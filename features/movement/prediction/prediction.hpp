#pragma once
#include "../../../sdk/sdk.hpp"

namespace prediction {
	void begin( c_usercmd* cmd );
	void end();
	void restore_ent_to_predicted_frame(int i);
	void post_think();

	inline player_move_data data{};
	inline c_usercmd* last_command;
	inline int* prediction_random_seed;
	inline int* prediction_player;
	inline int correct_tickbase;
	inline bool in_first;
	inline bool in_pred;
	inline bool in_prediction;
	
	inline vec3_t origin;
	inline vec3_t velocity;
	inline vec3_t original_angle;
	inline int flags;

	inline float old_cur_time;
	inline float old_frame_time;
	inline int old_tick_count;
}