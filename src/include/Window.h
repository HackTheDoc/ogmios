#pragma once

#include "Manager.h"
#include "Event.h"
#include "UI.h"
#include "Editor.h"

class Window {
private:
    SDL_Window* window;

public:
    static const std::string Title;
    static bool isRunning;

    static SDL_Renderer* renderer;
    static SDL_Rect screen;

    static Event event;

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
    void resize(const int w, const int h);
};
