#pragma once

#include <SDL2/SDL.h>

class Window;

class Event {
private:
    Window* w; // owner window

    void handleMouseEvents();
    void handleTextEditorEvents();

public:
    enum ID {
        UNKNOWN,

        SAVE,
        LOAD,
        MINUS_FONT_SIZE,
        PLUS_FONT_SIZE,
        DEFAULT_FONT_SIZE,
        CHANGE_THEME
    };
    SDL_Event e;

    Event();
    ~Event();

    void linkTo(Window* w);
    void handleKeyboardInputs();
    bool mouseClickLeft();
    bool mouseClickRight();
    bool controlKeyClicked();
    void raise(const Event::ID eid);
};