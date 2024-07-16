#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/movement/movement.hpp"

void __fastcall sdk::hooks::override_mouse_input::override_mouse_input( void* this_, int edx, float* x, float* y ) {
	if (features::movement::should_edge_bug) {

		*x *= 1 - c::movement::edge_bug_lock_amount / 100;
		*y *= 1 - c::movement::edge_bug_lock_amount / 100;
		return ofunc( this_, edx, x, y );
	}
	ofunc( this_, edx, x, y );
}