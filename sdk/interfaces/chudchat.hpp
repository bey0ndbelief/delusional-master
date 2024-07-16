#pragma once
#include <cstdarg>
#include <xlocinfo>
#include "../../utils/vfunc.hpp"

#define buffer_size 1024
class c_hud_chat {
public:
	void chatprintf(int iPlayerIndex, int iFilter, const char* format, ...) {
		static char buf[buffer_size] = "";
		va_list va;
		va_start(va, format);
		vsprintf_s(buf, buffer_size, format, va);
		va_end(va);
		get_vfunc<void(__cdecl*)(void*, int, int, const char*, ...)>(this, 27)(this, iPlayerIndex, iFilter, buf);
	}
};