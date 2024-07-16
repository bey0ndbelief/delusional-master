#include "draw.hpp"
#include "../../menu/fonts/icon.h"
#include "../../hooks/hooks.hpp"
#include "../../sdk/sdk.hpp"

void imgui_render::setup(IDirect3DDevice9* device) {
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(sdk::hooks::window);
	ImGui_ImplDX9_Init(device);
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(5.f, 10.f);
	style.FramePadding = ImVec2(5.f, 1.f);
	style.ItemSpacing = ImVec2(8.f, 5.f);
	style.WindowBorderSize = 1.f;
	style.ScrollbarSize = 0.f; // 
	style.GrabRounding = 1.f;
	style.ChildRounding = 1.f;
	style.FrameRounding = 1.f;
	style.WindowRounding = 1.f;
	style.PopupRounding = 1.f;
	style.GrabMinSize = 0.0f;
	style.FrameBorderSize = 0.f;

	style.Colors[ImGuiCol_WindowBg] = ImColor(13, 13, 13); //clr for main background
	style.Colors[ImGuiCol_FrameBg] = ImColor(24, 24, 24); //clr for multicombos/combos
	style.Colors[ImGuiCol_Border] = ImVec4(27 / 255.f, 27 / 255.f, 27 / 255.f, 1.f);
	style.Colors[ImGuiCol_Header] = ImColor(13, 13, 13); //
	style.Colors[ImGuiCol_HeaderActive] = ImColor(13, 13, 13); //
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(13, 13, 13); //
	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(20, 20, 20); //
	style.Colors[ImGuiCol_FrameBgActive] = ImColor(20, 20, 20); //
	style.Colors[ImGuiCol_SliderGrab] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);
	style.Colors[ImGuiCol_SliderGrabActive] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImFontConfig cfg_menu;
	cfg_menu.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;

	ImFontConfig cfg_screen;
	cfg_screen.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;

	char windows_directory[MAX_PATH];
	GetWindowsDirectoryA(windows_directory, MAX_PATH);

	std::string menu_font_directory = "C:/windows/fonts/verdana.ttf";
	std::string spec_font_directory = "C:/windows/fonts/tahoma.ttf";

	//menu
	fonts::menu_font_thin = io.Fonts->AddFontFromFileTTF(menu_font_directory.c_str(), 11, &cfg_menu, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//watermark
	fonts::watermark_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_watermark.c_str(), 11, &cfg_screen, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//sub spec
	ImFontConfig specfont_flag;
	menu::switch_font_cfg(specfont_flag, c::fonts::spec_font_flag);
	fonts::spec_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_sub_spec.c_str(), 11, &specfont_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	
	//main spec    
	fonts::main_spec_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_spec.c_str(), 11, &specfont_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//menu main
	fonts::menu_font_bold = io.Fonts->AddFontFromFileTTF(fonts::font_directory_menu_main.c_str(), 11, &cfg_menu, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//main indicators    
	ImFontConfig indifont_flag;
	menu::switch_font_cfg(indifont_flag, c::fonts::indi_font_flag);
	fonts::indicator_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_indicator.c_str(), c::fonts::indi_size, &indifont_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//sub indicators    
	ImFontConfig sub_indifont_flag;
	menu::switch_font_cfg(sub_indifont_flag, c::fonts::sub_indi_font_flag);
	fonts::sub_indicator_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_sub_indicator.c_str(), c::fonts::sub_indi_size, &sub_indifont_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//screen logs
	ImFontConfig sc_logs_font_flag;
	menu::switch_font_cfg(sc_logs_font_flag, c::fonts::sc_logs_flag);
	fonts::logs_font_flag = io.Fonts->AddFontFromFileTTF(fonts::font_directory_logs.c_str(), 11, &sc_logs_font_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp name
	ImFontConfig name_flag;
	menu::switch_font_cfg(name_flag, c::fonts::esp_name_font_flag);
	fonts::esp_name_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_name.c_str(), c::fonts::esp_name_size, &name_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp hp
	ImFontConfig hp_flag;
	menu::switch_font_cfg(hp_flag, c::fonts::esp_font_flag);
	fonts::esp_hp_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_health.c_str(), c::fonts::esp_hp_size, &hp_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp wpn
	ImFontConfig wpn_flag;
	menu::switch_font_cfg(wpn_flag, c::fonts::esp_wpn_font_flag);
	fonts::esp_wpn_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_wpn.c_str(), c::fonts::esp_wpn_size, &wpn_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp dropped wpn
	ImFontConfig dropped_wpn_flag;
	menu::switch_font_cfg(dropped_wpn_flag, c::fonts::esp_dropped_wpn_font_flag);
	fonts::esp_dropped = io.Fonts->AddFontFromFileTTF(fonts::font_directory_dropped_wpn.c_str(), c::fonts::esp_dropped_wpn_size, &dropped_wpn_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp misc
	fonts::esp_misc = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/Tahoma.ttf", 11, &cfg_screen, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//keystrokes misc
	fonts::key_strokes_font = io.Fonts->AddFontFromFileTTF("C:/windows/fonts/Tahoma.ttf", 12, &cfg_screen, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp icon
	constexpr ImWchar ranges[] = { 0xE000, 0xF8FF, 0 };

	ImFontConfig cfg_icon;
	cfg_icon.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_NoHinting;
	fonts::esp_wpn_icon = io.Fonts->AddFontFromMemoryCompressedTTF(icon_font, icon_font_size, 11.f, &cfg_icon, ranges);

	ImGuiFreeType::BuildFontAtlas(io.Fonts);
}

void imgui_render::drawline(const float x, const float y, const float x2, const float y2, const color_t& color, const float thickness) {
	m_draw_data.emplace_back(std::make_unique<drawing::Line>(ImVec2{ x, y }, ImVec2{ x2, y2 }, color_t::U32(color), thickness));
}

void imgui_render::text(const float x, const float y, const float fontSize, ImFont* font, const std::string& text, const bool centered, const color_t& color, const bool dropShadow, const bool outline) {
	m_draw_data.emplace_back(std::make_unique<drawing::TextSize>(fontSize, font, ImVec2{ x, y }, color_t::U32(color), text, dropShadow, centered, outline));
}

void imgui_render::drawrectfilled(const float x, const float y, const float w, const float h, const color_t& color, const ImDrawFlags flags) {
	m_draw_data.emplace_back(std::make_unique<drawing::RectangleFilled>(ImVec2{ x, y }, ImVec2{ x + w, y + h }, color_t::U32(color), 0.0f, flags));
}

void imgui_render::drawrectfilledgradient(const float x, const float y, const float w, const float h, const color_t& colUprLeft, const color_t& colUprRight, const color_t& colBotRight, const color_t& colBotLeft) {
	m_draw_data.emplace_back(std::make_unique<drawing::RectangleMultiColor>(ImVec2{ x, y }, ImVec2{ x + w, y + h },
		color_t::U32(colUprLeft), color_t::U32(colUprRight), color_t::U32(colBotRight), color_t::U32(colBotLeft)));
}

void imgui_render::drawrect(const float x, const float y, const float w, const float h, const color_t& color, const ImDrawFlags flags, const float thickness) {
	m_draw_data.emplace_back(std::make_unique<drawing::Rectangle>(ImVec2{ x, y }, ImVec2{ x + w, y + h }, color_t::U32(color), 0.0f, flags, thickness));
}

void imgui_render::drawcornerbox(const float x, const float y, const float w, const float h, const color_t& colour) {
	im_render.drawline(x, y, x, y + (h / c::visuals::corner_lenght), color_t(colour));
	im_render.drawline(x, y, x + (w / c::visuals::corner_lenght), y, color_t(colour));
	im_render.drawline(x + w, y, x + w - (w / c::visuals::corner_lenght), y, color_t(colour));
	im_render.drawline(x + w, y, x + w, y + (h / c::visuals::corner_lenght), color_t(colour));
	im_render.drawline(x, y + h, x + (w / c::visuals::corner_lenght), y + h, color_t(colour));
	im_render.drawline(x, y + h, x, y + h - (h / c::visuals::corner_lenght), color_t(colour));
	im_render.drawline(x + w, y + h, x + w - (w / c::visuals::corner_lenght), y + h, color_t(colour));
	im_render.drawline(x + w, y + h, x + w, y + h - (h / c::visuals::corner_lenght), color_t(colour));
}

void imgui_render::drawcircle(const float x, const float y, const float radius, const int points, const color_t& color, const float thickness) {
	m_draw_data.emplace_back(std::make_unique<drawing::Circle>(ImVec2{ x, y }, radius, points, color_t::U32(color), thickness));
}

void imgui_render::drawtrianglefilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const color_t& color) {
	m_draw_data.emplace_back(std::make_unique<drawing::TriangleFilled>(p1, p2, p3, color_t::U32(color)));
}

void imgui_render::drawpolygon(const std::vector<ImVec2>& verts, const color_t& color) {
	m_draw_data.emplace_back(std::make_unique<drawing::Polygon>(verts, color_t::U32(color)));
}

void imgui_render::drawpolyline(const std::vector<ImVec2>& verts, const color_t& color, const ImDrawFlags flags, const float thickness) {
	m_draw_data.emplace_back(std::make_unique<drawing::Polyline>(verts, color_t::U32(color), flags, thickness));
}

int imgui_render::get_text_width(const char* text, ImFont* font) {
	ImVec2 size = ImGui::CalcTextSize(text, NULL, true);
	return size.x;
}

ImVec2 imgui_render::get_text_size(ImFont* font, const std::string& text) {
	auto ret = ImGui::CalcTextSize(text.c_str());
	return ret;
}

void imgui_render::render_present(ImDrawList* draw) {
	std::unique_lock<std::shared_mutex> lock{ m_mutex };

	if (m_draw_data_safe.empty())
		return;

	for (const auto& data : m_draw_data_safe)
		data->draw(draw);
}

void imgui_render::clear_data() {
	if (!m_draw_data.empty())
		m_draw_data.clear();
}

void imgui_render::swawp_data() {
	std::unique_lock<std::shared_mutex> lock{ m_mutex };
	m_draw_data.swap(m_draw_data_safe);
}

void imgui_render::add_to_render(const std::function<void()>& fun) {
	clear_data();
	fun();
	swawp_data();
}

void fonts::reset_fonts( ) {
	// reset font variables to default values
	font_directory_watermark = "C:/windows/fonts/tahomabd.ttf";
	font_directory_menu_main = "C:/windows/fonts/tahomabd.ttf";
	font_directory_indicator = "C:/windows/fonts/tahomabd.ttf";
	font_directory_sub_indicator = "C:/windows/fonts/tahomabd.ttf";
	font_directory_misc = "C:/windows/fonts/tahomabd.ttf";
	font_directory_logs = "C:/windows/fonts/tahoma.ttf";
	font_directory_spec = "C:/windows/fonts/tahomabd.ttf";
	font_directory_sub_spec = "C:/windows/fonts/tahoma.ttf";
	font_directory_health = "C:/windows/fonts/tahoma.ttf";
	font_directory_name = "C:/windows/fonts/tahoma.ttf";
    font_directory_wpn = "C:/windows/fonts/tahoma.ttf";
	font_directory_dropped_wpn = "C:/windows/fonts/tahoma.ttf";

	// reset other font-related variables to default values
	c::fonts::indi_font = 0;
	c::fonts::sub_indi_font = 0;
	c::fonts::spec_font = 0;
	c::fonts::sub_spec_font = 0;
	c::fonts::sc_logs_font = 0;
	c::fonts::esp_name_font = 0;
	c::fonts::esp_hp_font = 0;
	c::fonts::esp_wpn_font = 0;
	c::fonts::esp_dropped_wpn_font = 0;

	//reset size
	c::fonts::indi_size = 28;
	c::fonts::sub_indi_size = 28;
	c::fonts::spec_size = 11;
	c::fonts::esp_name_size = 11;
	c::fonts::esp_hp_size = 11;
	c::fonts::esp_wpn_size = 11;
	c::fonts::esp_dropped_wpn_size = 11;
}

void imgui_render::clear_textures() {

}

void imgui_render::clear_unused_avatars() {

}

//

void ctexture::init(int width, int height, const std::uint8_t* data)  {
	texture = ImGui_CreateTextureRGBA(width, height, data);
}

void ctexture::clear() {
	if (texture)
		ImGui_DestroyTexture(texture);
	texture = nullptr;
}
