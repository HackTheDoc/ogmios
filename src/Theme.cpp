#include "include/Theme.h"

#include "include/Manager.h"

#include <nlohmann/json.hpp>
#include <fstream>

#include <iostream>

Theme::Theme() {
    next = Theme::Type::DAY;

    font = {255,255,255,255};
    cursor = {255,255,255,255};
    ui = {255,255,255,255};
    uiBackground = {0,0,0,255};
    textBackground = {0,0,0,255};

    icon = "";

    fontSize = 0;
}

Theme::~Theme() {}

void Theme::load(Theme::Type t) {
    std::string filepath = "./themes/";
    switch (t) {
    case Theme::Type::NIGHT:
        filepath += "night.json";
        next = Theme::Type::DAY;
        break;
    case Theme::Type::DAY:
    default:
        filepath += "day.json";
        next = Theme::Type::NIGHT;
        break;
    }

    std::fstream infile(filepath);
    nlohmann::json data;
    infile >> data;
    infile.close();

    auto fc = data["font color"];
    font.r = fc[0];
    font.g = fc[1];
    font.b = fc[2];
    font.a = fc[3];

    auto cc = data["cursor color"];
    cursor.r = cc[0];
    cursor.g = cc[1];
    cursor.b = cc[2];
    cursor.a = cc[3];

    auto uc = data["ui color"];
    ui.r = uc[0];
    ui.g = uc[1];
    ui.b = uc[2];
    ui.a = uc[3];

    auto ubc = data["ui background color"];
    uiBackground.r = ubc[0];
    uiBackground.g = ubc[1];
    uiBackground.b = ubc[2];
    uiBackground.a = ubc[3];

    auto tbc = data["text background color"];
    textBackground.r = tbc[0];
    textBackground.g = tbc[1];
    textBackground.b = tbc[2];
    textBackground.a = tbc[3];

    auto sc = data["selection highlighting color"];
    selection.r = sc[0];
    selection.g = sc[1];
    selection.b = sc[2];
    selection.a = sc[3];

    icon = data["icon"];
}
