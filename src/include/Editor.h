#pragma once

#include <vector>

#include "UILine.h"

struct Data;

class Editor {
private:
    SDL_Rect container;
    SDL_Rect cursor;

    int selectionLength;
    bool lineSelected;

    int scrollPosition;

    std::string currentFile; // path to the current file editing
    bool fileSaved; // represent wether the current file is saved or not

    void saveToNewFile();
    void saveToCurrentFile();

public:
    static int LeftMargin;
    static int LineHeight;
    static SDL_Point Cursor;
    static std::vector<UILine*> lines;

    static std::vector<std::string> Format();

    Editor(const int w, const int h);
    ~Editor();
    
    void importData(const Data& d);
    Data exportData();

    void init();
    void update();
    void render();
    void clear();
    void destroy();
    void reload();

    void place(const int x, const int y);
    void setWidth(const int w);

    void updateCursorPlacement();
    void updateFontSize(const int s);
    void resetFontSize();

    void insertChar(const char c);
    void insertTab();
    void deletePreviousChar();
    void deleteNextChar();
    void deletePreviousWord();
    void deleteNextWord();

    void select(const int d);
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

    void save(const bool isnew = false);
    void load();

    void createNewFile(const bool checkIfSaved = true);
};
