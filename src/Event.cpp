#include "include/Event.h"

#include "include/Window.h"
#include "include/Config.h"

Event::Event() : w(nullptr) {}

Event::~Event() {}

void Event::linkTo(Window* w) {
    this->w = w;
}

void Event::handleKeyboardInputs() {
    switch (e.type) {
    case SDL_WINDOWEVENT:
        if (e.window.event == SDL_WINDOWEVENT_RESIZED)
            w->resize(e.window.data1, e.window.data2);
        break;
    case SDL_TEXTINPUT:
        Window::editor->insertChar(*e.text.text);
        break;
    case SDL_KEYDOWN:
        handleTextEditorEvents();
        break;
    case SDL_MOUSEBUTTONUP:
        handleMouseEvents();
        break;
    default:
        break;
    }
}

bool Event::mouseClickLeft() {
    return e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT;
}

bool Event::mouseClickRight() {
    return e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT;
}

bool Event::controlKeyClicked() {
    return SDL_GetModState() & KMOD_CTRL;
}

void Event::raise(const Event::ID eid) {
    switch (eid) {
    case ID::SAVE:
        Window::editor->save();
        break;
    case ID::LOAD:
        Window::editor->load();
        break;
    case ID::MINUS_FONT_SIZE:
        if (controlKeyClicked())
            Window::editor->updateFontSize(Config::FONT_MIN_SIZE - Config::fontSize);
        else
            Window::editor->updateFontSize(-2);
        break;
    case ID::PLUS_FONT_SIZE:
        if (controlKeyClicked())
            Window::editor->updateFontSize(Config::FONT_MAX_SIZE - Config::fontSize);
        else
            Window::editor->updateFontSize(2);
        break;
    case ID::DEFAULT_FONT_SIZE:
        Window::editor->resetFontSize();
        break;
    case ID::CHANGE_THEME:
        w->switchTheme();
        break;
    case ID::UNKNOWN:
    default:
        break;
    }
}

void Event::handleTextEditorEvents() {
    switch (e.key.keysym.sym) {
    case SDLK_UP:
        if (controlKeyClicked())
            Window::editor->scroll(-1);
        else
            Window::editor->moveCursorUp();
        break;
    case SDLK_DOWN:
        if (controlKeyClicked())
            Window::editor->scroll(1);
        else
            Window::editor->moveCursorDown();
        break;
    case SDLK_LEFT:
        if (controlKeyClicked())
            Window::editor->select(-1);
        else
            Window::editor->moveCursorLeft();
        break;
    case SDLK_RIGHT:
        if (controlKeyClicked())
            Window::editor->select(1);
        else
            Window::editor->moveCursorRight();
        break;
    case SDLK_HOME:
        Window::editor->jumpToLineStart();
        break;
    case SDLK_END:
        Window::editor->jumpToLineEnd();
        break;
    case SDLK_PAGEUP:
        Window::editor->jumpToFileStart();
        break;
    case SDLK_PAGEDOWN:
        Window::editor->jumpToFileEnd();
        break;
    case SDLK_BACKSPACE:
        if (!Window::editor->deleteSelection())
            if (!Window::editor->deleteCurrentLine())
                Window::editor->deletePreviousChar();
        break;
    case SDLK_DELETE:
        if (!Window::editor->deleteNextLine())
            Window::editor->deleteNextChar();
        break;
    case SDLK_RETURN:   // NEW LINE
        Window::editor->insertNewLine();
    case SDLK_c:        // CTRL C
        if (controlKeyClicked())
            Window::editor->setClipboardText();
        break;
    case SDLK_v:        // CTRL V
        if (controlKeyClicked())
            Window::editor->pasteClipboardText();
        break;
    case SDLK_s:        // CTRL S
        if (controlKeyClicked()) {
            if (SDL_GetModState() & KMOD_SHIFT)
                Window::editor->save(true);
            else
                Window::editor->save();
        }
        break;
    case SDLK_o:        // CTRL O
        if (controlKeyClicked())
            Window::editor->load();
        break;
    case SDLK_n:        // CTRL N
        if (controlKeyClicked())
            Window::editor->createNewFile();
        break;
    case SDLK_l:        // CTRL L
        if (controlKeyClicked())
            Window::editor->selectLine();
        break;
    default:
        break;
    }
}

void Event::handleMouseEvents() {
    // none
}
