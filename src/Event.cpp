#include "include/Event.h"

#include "include/Window.h"

Event::Event() : o(nullptr) {}

Event::~Event() {}

void Event::handleKeyboardInputs() {
    switch (e.type) {
    case SDL_WINDOWEVENT:
        if (e.window.event == SDL_WINDOWEVENT_RESIZED)
            o->resize(e.window.data1, e.window.data2);
        break;
    case SDL_TEXTINPUT:
        Window::editor->insertChar(*e.text.text);
        break;
    case SDL_KEYDOWN:
        handleTextEditorEvents(e.key.keysym.sym);
        break;
    case SDL_MOUSEBUTTONUP:
        handleMouseEvents();
        break;
    default:
        break;
    }
}

void Event::linkTo(Window* w) {
    o = w;
}

void Event::handleButtonClick(UIButton::ID id) {
    switch (id) {
    case UIButton::ID::SAVE:
        o->editor->save();
        break;
    case UIButton::ID::LOAD:
        o->editor->load();
        break;
    case UIButton::ID::MINUS_SIZE:
        o->editor->updateFontSize(-2);
        break;
    case UIButton::ID::DEFAULT_SIZE:
        o->editor->updateFontSize(Manager::DEFAULT_FONT_SIZE - Window::theme.fontSize);
        break;
    case UIButton::ID::PLUS_SIZE:
        o->editor->updateFontSize(2);
        break;
    case UIButton::ID::THEME_ICON:
        o->switchTheme();
        break;
    case UIButton::ID::UNKNOWN:
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

void Event::handleTextEditorEvents(SDL_Keycode key) {
        switch (key) {
        case SDLK_UP:
            if (SDL_GetModState() & KMOD_CTRL)
                o->editor->scroll(-1);
            else
                o->editor->moveCursorUp();
            break;
        case SDLK_DOWN:
            if (SDL_GetModState() & KMOD_CTRL)
                o->editor->scroll(1);
            else
                o->editor->moveCursorDown();
            break;
        case SDLK_LEFT:
            if (SDL_GetModState() & KMOD_CTRL)
                o->editor->select(-1);
            else 
                o->editor->moveCursorLeft();
            break;
        case SDLK_RIGHT:
            if (SDL_GetModState() & KMOD_CTRL)
                o->editor->select(+1);
            else 
                o->editor->moveCursorRight();
            break;
        case SDLK_HOME:
            o->editor->jumpToLineStart();
            break;
        case SDLK_END:
            o->editor->jumpToLineEnd();
            break;
        case SDLK_PAGEUP:
            o->editor->jumpToFileStart();
            break;
        case SDLK_PAGEDOWN:
            o->editor->jumpToFileEnd();
            break;   
        case SDLK_BACKSPACE:        // SUPPR (selection, current line, previous char)
            if (!o->editor->deleteSelection())
                if (!o->editor->deleteCurrentLine())
                    o->editor->deletePreviousChar();
            break;
        case SDLK_DELETE:           // DELETE (next line, next char)
            if (!o->editor->deleteNextLine()) {
                o->editor->deleteNextChar();
            }
            break;
        case SDLK_RETURN:           // NEW LINE
            o->editor->insertNewLine();
            break;
        case SDLK_TAB:
            o->editor->insertTab(); // INSERT TAB (NOT WORKING)
            break;
        case SDLK_c:                // COPY
            if (SDL_GetModState() & KMOD_CTRL)
                o->editor->setClipboardText();
            break;
        case SDLK_v:                // PASTE
            if (SDL_GetModState() & KMOD_CTRL)
                o->editor->pasteClipboardText();
            break;
        case SDLK_s:                // SAVE
            if (SDL_GetModState() & KMOD_CTRL)
                o->editor->save();
            break;
        case SDLK_o:                // OPEN
            if (SDL_GetModState() & KMOD_CTRL)
                o->editor->load();
            break;
        case SDLK_l:                // SELECT LINE
            if (SDL_GetModState() & KMOD_CTRL)
                o->editor->selectLine();
            break;
        default:
            break;
    }
}

void Event::handleMouseEvents() {

}
