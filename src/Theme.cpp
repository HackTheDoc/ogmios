#include "include/Theme.h"

#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

std::string Theme::name = "unknown";

SDL_Color Theme::clr_font = { 255, 255, 255, 255 };
SDL_Color Theme::clr_cursor = { 255, 255, 255, 255 };
SDL_Color Theme::clr_ui = { 255, 255, 255, 255 };
SDL_Color Theme::clr_ui_background = { 0, 0, 0, 255 };
SDL_Color Theme::clr_editor_background = { 0, 0, 0, 255 };
SDL_Color Theme::clr_selection = { 255, 0, 0, 255 };

std::string Theme::icon = "";

void Theme::load(const std::string& name) {
    if (name == Theme::name) return;

    Theme::name = name;

    std::fstream infile("./themes.json");
    json rdata;
    infile >> rdata;
    infile.close();

    json data = rdata[name];

    auto fc = data["font color"];
    clr_font.r = fc[0];
    clr_font.g = fc[1];
    clr_font.b = fc[2];
    clr_font.a = fc[3];

    auto cc = data["cursor color"];
    clr_cursor.r = cc[0];
    clr_cursor.g = cc[1];
    clr_cursor.b = cc[2];
    clr_cursor.a = cc[3];

    auto uc = data["ui color"];
    clr_ui.r = uc[0];
    clr_ui.g = uc[1];
    clr_ui.b = uc[2];
    clr_ui.a = uc[3];

    auto ubc = data["ui background color"];
    clr_ui_background.r = ubc[0];
    clr_ui_background.g = ubc[1];
    clr_ui_background.b = ubc[2];
    clr_ui_background.a = ubc[3];

    auto tbc = data["editor background color"];
    clr_editor_background.r = tbc[0];
    clr_editor_background.g = tbc[1];
    clr_editor_background.b = tbc[2];
    clr_editor_background.a = tbc[3];

    auto sc = data["selection highlighting color"];
    clr_selection.r = sc[0];
    clr_selection.g = sc[1];
    clr_selection.b = sc[2];
    clr_selection.a = sc[3];

    icon = data["icon path"];
}
