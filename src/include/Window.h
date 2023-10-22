#pragma once

#include <string>
#include <SDL2/SDL.h>

#include "Manager.h"
#include "Event.h"
#include "Theme.h"
#include "UI.h"
#include "Editor.h"

class Window {
public:
    static const int MIN_WIDTH;
    static const int MIN_HEIGHT;
    static const int DEFAULT_WIDTH;
    static const int DEFAULT_HEIGHT;

    static const std::string TITLE;

    static bool isRunning;
    static SDL_Renderer* renderer;
    static SDL_Rect screen;
    static Manager manager;
    static Event event;
    static Theme theme;

    static UI* ui;
    static Editor* editor;

    static void Reload();

    Window();
    ~Window();

    int init();
    void handleEvents();
    void update();
    void render();
    void kill();

    void switchTheme();
    void resize(int w, int h);
private:
    SDL_Window* window;
};
