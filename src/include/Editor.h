#pragma once

#include <vector>
#include <SDL2/SDL.h>

#include "UILine.h"

class Editor {
public:
    static const int DEFAULT_LEFT_MARGIN;
    static const int DEFAULT_LINE_HEIGHT;

    static int LeftMargin;
    static int LineHeight;
    static SDL_Point Cursor;
    static std::vector<UILine*> lines;

    Editor(int w, int h);
    ~Editor();

    void init();
    void update();
    void render();
    void clear();
    void destroy();
    void reload();

    void place(int x, int y);
    void setWidth(int w);

    void updateCursorPlacement();
    void updateFontSize(int s);
    void resetFontSize();

    void insertChar(char c);
    void insertTab();
    void deletePreviousChar();
    void deleteNextChar();
    void deletePreviousWord();
    void deleteNextWord();

    void select(int d);
    void selectLine();
    bool deleteSelection();

    void insertNewLine();
    bool deleteCurrentLine();
    bool deleteNextLine();

    void jumpToFileStart();
    void jumpToFileEnd();
    void jumpToLineStart();
    void jumpToLineEnd();

    void moveCursorUp();
    bool moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();
    void scroll(int s);

    void setClipboardText();
    void pasteClipboardText();

    void save();
    void load();

private:
    SDL_Rect viewport;
    SDL_Rect cursorRect;

    int selectionLength;
    bool lineSelected;

    int scrollPosition;
};
