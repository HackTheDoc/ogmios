#include "include/Editor.h"

#include "include/Window.h"
#include "include/tinyfiledialogs.h"

#include <fstream>
#include <iostream>

const int Editor::DEFAULT_LEFT_MARGIN = 24;
const int Editor::DEFAULT_LINE_HEIGHT = 22;

int Editor::LeftMargin = Editor::DEFAULT_LEFT_MARGIN;
int Editor::LineHeight = Editor::DEFAULT_LINE_HEIGHT;
SDL_Point Editor::Cursor = {0,0};
std::vector<UILine*> Editor::lines = {};

Editor::Editor(int w, int h) {
    viewport = {0,0,w,h};
    cursorRect = {0,0,1,LineHeight};

    selectionLength = 0;
    lineSelected = false;

    scrollPosition = 0;
}

Editor::~Editor() {}

void Editor::init() {
    UILine* l = new UILine("", 0);
    lines.push_back(l);

    jumpToFileStart();
}

void Editor::update() {
    viewport.y = -scrollPosition * LineHeight + Window::ui->height();

    for (auto l : lines)
        l->update();
}

void Editor::render() {
    Manager::SetViewport(&viewport);

    for (auto l : lines)
        l->draw();

    Manager::DrawRect(&cursorRect, Window::theme.cursor);

    if (selectionLength != 0) {
        int length = abs(selectionLength);
        UILine* l = lines[Cursor.y];
        
        int x = Cursor.x;
        if (selectionLength > 0) x -= length;

        std::string s = l->getText().substr(x, length);

        SDL_Rect selection = {cursorRect.x, cursorRect.y+2, 0, cursorRect.h-2};

        TTF_SizeText(Manager::font, s.c_str(), &selection.w, nullptr); 
        if (selectionLength > 0) selection.x -= selection.w;

        Manager::DrawFilledRect(&selection, Window::theme.selection);
    }

    Manager::SetViewport(nullptr);
}

void Editor::clear() {
    for (auto l : lines)
        l->destroy();
    lines.clear();

    init();
}

void Editor::destroy() {
    for (auto l : lines)
        l->destroy();
    lines.clear();
}

void Editor::reload() {
    for (auto l : lines)
        l->reload();

    updateCursorPlacement();
}

void Editor::place(int x, int y) {
    viewport.x = x;
    viewport.y = y;
}

void Editor::setWidth(int w) {
    viewport.w = w;
    reload();
}

void Editor::updateCursorPlacement() {
    UILine* l = lines[Cursor.y];

    std::string t = l->getText().substr(0, Cursor.x);
    TTF_SizeText(Manager::font, t.c_str(), &cursorRect.x, nullptr);
    cursorRect.x += Editor::LeftMargin;

    cursorRect.y = Cursor.y * LineHeight + 4;
    cursorRect.h = l->height() - 4;
}

void Editor::updateFontSize(int s) {
    Window::theme.fontSize += s;

    Manager::SetFontSize(Window::theme.fontSize);

    LineHeight += s;
    LeftMargin += s;

    reload();
}

void Editor::insertChar(char c) {
    lines[Cursor.y]->insert(Cursor.x, c);
    Cursor.x++;
    selectionLength = 0;
    updateCursorPlacement();
}

void Editor::insertTab() {
    lines[Cursor.y]->insert(Cursor.x, '\t');
    Cursor.x++;
    selectionLength = 0;
    updateCursorPlacement();
}

void Editor::deletePreviousChar() {
    if (Cursor.x > 0) {
        lines[Cursor.y]->erase(Cursor.x - 1, 1);
        Cursor.x--;

        updateCursorPlacement();
    }
}

void Editor::deleteNextChar() {
    if (Cursor.x >= static_cast<int>(lines[Cursor.y]->size())) return
    
    lines[Cursor.y]->erase(Cursor.x, 1);
}

/// TODO:
void Editor::deletePreviousWord() {

}

/// TODO:
void Editor::deleteNextWord() {

}

void Editor::select(int d) {
    if (d < 0 && Cursor.x <= 0) return;
    if (d > 0 && Cursor.x >= lines[Cursor.y]->size()) return;

    selectionLength += d;

    Cursor.x += d;
    updateCursorPlacement();
}

void Editor::selectLine() {
    lineSelected = true;

    UILine* l = lines[Cursor.y];

    Cursor.x = l->size();

    selectionLength = l->size();

    updateCursorPlacement();
}

bool Editor::deleteSelection() {
    if (selectionLength == 0) return false;

    if (lineSelected) {
        lineSelected = false;

        UILine* l = lines[Cursor.y];
        std::string oldLine = l->getText();

        l->destroy();
        lines.erase(lines.begin() + Cursor.y);

        for (int i = Cursor.y; i < (int)lines.size(); i++) {
            lines[i]->setNumber(i);
        }

        moveCursorUp();
        l = lines[Cursor.y];
        Cursor.x = (int)l->size();
        updateCursorPlacement();

        viewport.h -= LineHeight;

        return true;
    }

    int length = abs(selectionLength);
    UILine* l = lines[Cursor.y];

    if (selectionLength > 0) Cursor.x -= length;

    l->erase(Cursor.x, length);

    selectionLength = 0;

    updateCursorPlacement();

    return true;
}

void Editor::insertNewLine() {
    UILine* l = lines[Cursor.y];

    if (lineSelected) {
        lineSelected = false;
        l->clear();
        if (Cursor.y >= (int)lines.size() - 1)
            jumpToLineStart();
        else
            moveCursorRight();
        return;
    }        
    else if (selectionLength != 0) 
        deleteSelection();

    std::string text = l->getText();

    std::string newLine = text.substr(Cursor.x);
    std::string oldLine = text.substr(0, Cursor.x);

    l->setText(oldLine);

    UILine* nl = new UILine(newLine, Cursor.y+1);
    lines.insert(lines.begin() + Cursor.y + 1, nl);

    for (int i = nl->getNumber()+1; i < (int)lines.size(); i++) {
        UILine* l = lines[i];
        l->setNumber(i);
    }

    moveCursorDown();
    Cursor.x = 0;
    
    updateCursorPlacement();

    viewport.h += LineHeight;
}

bool Editor::deleteCurrentLine() {
    if (Cursor.x != 0 || Cursor.y <= 0) return false;

    UILine* l = lines[Cursor.y];
    std::string oldLine = l->getText();

    l->destroy();
    lines.erase(lines.begin() + Cursor.y);

    for (int i = Cursor.y; i < (int)lines.size(); i++) {
        lines[i]->setNumber(i);
    }

    moveCursorUp();
    l = lines[Cursor.y];
    Cursor.x = (int)l->size();
    updateCursorPlacement();

    if (!oldLine.empty()) {
        l->append(oldLine);
    }

    viewport.h -= LineHeight;

    return true;
}

bool Editor::deleteNextLine() {
    UILine* l = lines[Cursor.y];

    if (Cursor.x != static_cast<int>(l->size()) || Cursor.y >= static_cast<int>(lines.size() - 1)) 
        return false;

    UILine* lToDelete = lines[Cursor.y + 1];
    std::string oldLine = lToDelete->getText();

    lToDelete->destroy();
    lines.erase(lines.begin() + Cursor.y + 1);
    
    if (!oldLine.empty()) {
        l->append(oldLine);
    }

    viewport.h -= LineHeight;

    return true;
}

void Editor::jumpToFileStart() {
    Cursor.y = 0;
    jumpToLineStart();

    scrollPosition = 0;
}

void Editor::jumpToFileEnd() {
    while (moveCursorDown()) {}
    jumpToLineEnd();
}

void Editor::jumpToLineStart() {
    Cursor.x = 0;
    updateCursorPlacement();
    
    selectionLength = 0;
}

void Editor::jumpToLineEnd() {
    Cursor.x = static_cast<int>(lines[Cursor.y]->size());
    updateCursorPlacement();

    selectionLength = 0;
}

void Editor::moveCursorUp() {
    if (Cursor.y <= 0) return;

    selectionLength = 0;

    Cursor.y--;
    Cursor.x = std::min(Cursor.x, static_cast<int>(lines[Cursor.y]->size()));

    if (Cursor.y  < scrollPosition) {
        scroll(-1);
    }

    updateCursorPlacement();
}

bool Editor::moveCursorDown() {
    if (Cursor.y >= static_cast<int>(lines.size() - 1) ) return false;

    selectionLength = 0;

    Cursor.y++;
    Cursor.x = std::min(Cursor.x, static_cast<int>(lines[Cursor.y]->size()));

    if ((Cursor.y - scrollPosition + 1) * LineHeight > Window::screen.h - Window::ui->height()) {
        scroll(1);
    }

    updateCursorPlacement();

    return true;
}

void Editor::moveCursorLeft() {
    if (Cursor.x == 0 && Cursor.y > 0) {
        moveCursorUp();
        Cursor.x = static_cast<int>(lines[Cursor.y]->size());
    }
    else if (Cursor.x > 0) {
        Cursor.x--;
    }
    selectionLength = 0;

    updateCursorPlacement();
}

void Editor::moveCursorRight() {
    if (Cursor.x >= (int)lines[Cursor.y]->size() && Cursor.y < (int)lines.size()-1) {
        moveCursorDown();
        Cursor.x = 0;
    }
    else if (Cursor.x < (int)lines[Cursor.y]->size()) {
        Cursor.x++;
    }
    selectionLength = 0;
    updateCursorPlacement();
}

void Editor::scroll(int s) {
    scrollPosition += s;

    scrollPosition = std::max(0, std::min(scrollPosition, static_cast<int>(lines.size()-1)));
}

void Editor::save() {
    char const * filterPatterns[2] = { "*.txt", "*.text" };
    char* path = tinyfd_saveFileDialog("Save", "output/unknown.txt", 2, filterPatterns, NULL);
    
    if (path != NULL) {
        std::ofstream outfile(path);
        for (auto l : lines) {
            std::string t = l->getText();
            outfile << t << std::endl;
        }
        outfile.close();
    } else {
        tinyfd_messageBox("Ogmios", "Cannot save the file !", "ok", "error", 1);
    }
}

void Editor::load() {
    char const * filterPatterns[2] = { "*.txt", "*.text" };
    char* path = tinyfd_openFileDialog("Open", "output/unknown.txt", 2, filterPatterns, NULL, 0);

    if (path != NULL) {
        lines.clear();
        std::ifstream infile(path);
        std::string t;
        int n = 0;
        while (std::getline(infile, t)) {
            UILine* l = new UILine(t, n);
            lines.push_back(l);
        }
        infile.close();

        jumpToFileEnd();
    } else {
        tinyfd_messageBox("Ogmios", "Cannot open the file !", "ok", "error", 1);
    }
}

void Editor::setClipboardText() {
    if (selectionLength == 0) {
        SDL_SetClipboardText("");
        return;
    }

    int length = abs(selectionLength);
    UILine* l = lines[Cursor.y];

    int x = Cursor.x;
    if (selectionLength > 0) x -= length;

    std::string s = l->getText().substr(x, length);

    SDL_SetClipboardText(s.c_str());
}

void Editor::pasteClipboardText() {
    char* cbt = SDL_GetClipboardText();
    
    UILine* l = lines[Cursor.y];
    l->insert(Cursor.x, cbt);

    std::string t(cbt);
    Cursor.x += t.length();
    updateCursorPlacement();

    SDL_free(cbt);
}
