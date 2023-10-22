#pragma once

#include <SDL2/SDL.h>

#include "UIButton.h"

class Window;

class Event {
public:
    SDL_Event e;

    Event();
    ~Event();

    void linkTo(Window* w);

    void handleKeyboardInputs();

    bool mouseClickLeft();

    bool mouseClickRight();

    void handleButtonClick(UIButton::ID id);

private:
    // owner window
    Window* o;

    void handleTextEditorEvents(SDL_Keycode key);
    void handleMouseEvents();
};
