#include "../hooks.hpp"
#include "../../menu/menu.hpp"
#include "../../features/misc/misc.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/movement/movement.hpp"
#include "../../includes/discord/discord_rpc.h"
#include "../../sdk/math/math.hpp"
#include <TlHelp32.h>

LRESULT __stdcall sdk::hooks::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	if (message == WM_KEYDOWN && LOWORD(wparam) == c::misc::menu_key)
		menu::open = !menu::open;

	LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	ImGui_ImplWin32_WndProcHandler(window, message, wparam, lparam);

	return CallWindowProcW(wndproc_original, hwnd, message, wparam, lparam);
}

std::wstring ttl = L"";
DWORD procID = 0;

bool is_spotify(const PROCESSENTRY32W& entry) {
	return std::wstring(entry.szExeFile) == L"Spotify.exe";
}

BOOL __stdcall enum_windows_proc(HWND hwnd, LPARAM lParam) {
	const auto& pids = *reinterpret_cast<std::vector<DWORD>*>(lParam);

	DWORD winId;
	GetWindowThreadProcessId(hwnd, &winId);

	if (pids.empty())
		return FALSE;

	for (DWORD pid : pids) {
		if (winId == pid) {
			std::wstring title(GetWindowTextLength(hwnd) + 1, L'\0');
			GetWindowTextW(hwnd, &title[0], title.size()); //note: C++11 only
			if (wcsstr(title.c_str(), L"GDI+ Window (Spotify.exe)") || wcsstr(title.c_str(), L"Default IME") || title.empty() || wcsstr(title.c_str(), L"MSCTFIME UI")) {
				continue;
			}
			if (lstrlenW(title.c_str()) > 1) {
				procID = pid;
				ttl = title;
			}
		}
	}

	return TRUE;
}

bool handle_spotify_window() {
	std::vector<DWORD> pids; //process ID's

	//get handle of our proc
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//get entry point
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(entry);

	//see if our snapshot is valid
	if (!Process32FirstW(snap, &entry)) {
		return false;
	}

	//check if the handle is to spotify or not
	do {
		if (is_spotify(entry)) {
			pids.emplace_back(entry.th32ProcessID);
		}
	} while (Process32NextW(snap, &entry));


	//get enumeration of processes
	return EnumWindows(enum_windows_proc, reinterpret_cast<LPARAM>(&pids));
}

std::string spotifytitle = "";
std::string spotifytitle2 = "";
float lasttime = 0.f;
void show_misc_info() {
	if (interfaces::globals->realtime > lasttime + 1.5f) {
		if (handle_spotify_window()) {
			std::string ttl2;
			if (char title[128]; WideCharToMultiByte(CP_UTF8, 0, ttl.c_str(), -1, title, 128, nullptr, nullptr)) {
				ttl2 = std::string(title);
			}
			spotifytitle = wcsstr(ttl.c_str(), L"spotify") ? " stopped / not playing" : ttl2;

			if (strstr(spotifytitle.c_str(), "Spotify Premium"))
				spotifytitle = " stopped / not playing";

			if (strstr(spotifytitle.c_str(), "Spotify Free"))
				spotifytitle = " stopped / not playing";

			if (strstr(spotifytitle.c_str(), "Advertisement"))
				spotifytitle = " advertisement";
		}
		else {
			spotifytitle = " spotify not open / not running";
		}

		lasttime = interfaces::globals->realtime;
	}

	if (c::misc::show_spotify_currently_playing) {
		int w, h;
		interfaces::engine->get_screen_size(w, h);

		h = 5;

		ImGui::GetBackgroundDrawList()->AddText(fonts::main_spec_font, 11.f, ImVec2(w - 5 - im_render.get_text_width(spotifytitle.c_str(), fonts::menu_font_bold) + 1, h + 1), ImColor(0, 0, 0, 255), spotifytitle.c_str());
		ImGui::GetBackgroundDrawList()->AddText(fonts::main_spec_font, 11.f, ImVec2(w - 5 - im_render.get_text_width(spotifytitle.c_str(), fonts::menu_font_bold), h), ImColor(255, 255, 255, 255), spotifytitle.c_str());
	}

	static float frame_rate = 0;
	frame_rate = 0.9f * frame_rate + (1.f - 0.9f) * interfaces::globals->absolute_frametime;
	int get_fps = (1.f / frame_rate);

	std::string output = "";
	static std::string main = "delusional";
	std::string get_frametime = std::to_string(get_fps);
	std::string fps = " | " + get_frametime + " fps";
	/*
	* if (c::misc::watermark) {
		output += main + fps;
	}

	if (c::misc::watermark) {
		int w, h;
		auto text_size = ImGui::CalcTextSize(output.c_str());
		static const ImVec2& padding = ImVec2(7, 7);
		static const ImVec2& margin = ImVec2(10, 6); //
		static const int& box_offset = 10;
		interfaces::engine->get_screen_size(w, h);

		const std::vector<ImVec2>& points = {
			ImVec2(w - text_size.x - padding.x - margin.x * 2.f, padding.y),
			ImVec2(w - padding.x + box_offset, padding.y),
			ImVec2(w - padding.x, text_size.y + padding.y + margin.y * 2.f),
			ImVec2(w - text_size.x - padding.x - margin.x * 2.f - box_offset, text_size.y + padding.y + margin.y * 2.f),
		};


		ImGui::GetBackgroundDrawList()->AddRectFilled({ w - text_size.x - padding.x - margin.x * 2.f  ,  padding.y }, { w - padding.x, text_size.y + padding.y + margin.y * 2.f }, ImColor(10, 10, 10, 255), 0.f);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(w - text_size.x - padding.x - margin.x * 2.f, text_size.y + padding.y + margin.y * 2.f + 0.5), ImVec2(w - padding.x, text_size.y + padding.y + margin.y * 2.f + 0.5), ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f), 1.f);

		ImGui::GetBackgroundDrawList()->AddText(fonts::watermark_font, 11.f, ImVec2(w - text_size.x - padding.x - margin.x + 1, padding.y + margin.y), ImColor(0, 0, 0, 255), main.c_str());
		ImGui::GetBackgroundDrawList()->AddText(fonts::watermark_font, 11.f, ImVec2(w - text_size.x - padding.x - margin.x, padding.y + margin.y), ImColor(menu::menu_accent2[0], menu::menu_accent2[1], menu::menu_accent2[2]), main.c_str());

		ImGui::GetBackgroundDrawList()->AddText(fonts::menu_font_thin, 11.f, ImVec2(w - text_size.x - padding.x - margin.x + 51, padding.y + margin.y + 1), ImColor(0, 0, 0, 255), fps.c_str());
		ImGui::GetBackgroundDrawList()->AddText(fonts::menu_font_thin, 11.f, ImVec2(w - text_size.x - padding.x - margin.x + 50, padding.y + margin.y), ImColor(140, 140, 140, 255), fps.c_str());
	}
	*/
}

long __stdcall sdk::hooks::present::present(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region) {
	static auto once = [=]() {
		im_render.setup(device);
		return true;
	} ();

	if (menu::fonts_initialized) {
		im_render.setup(device);
		menu::fonts_initialized = false;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	{
		const auto im_draw = ImGui::GetBackgroundDrawList();

		im_render.render_present(im_draw);
		features::visuals::removals();
		features::visuals::fullbright();
		features::misc::draw();
		features::misc::spectators_list();
		show_misc_info();
		menu::render();
		c_discord::get().update();
	}
	ImGui::EndFrame();
	ImGui::Render();

	if (device->BeginScene() == D3D_OK) {
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	return ofunc(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

LRESULT __stdcall sdk::hooks::reset::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp) {
	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto result = ofunc(device, pp);

	ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}