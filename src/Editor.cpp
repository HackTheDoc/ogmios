#include "include/Editor.h"

#include "include/File.h"
#include "include/Window.h"
#include "include/tinyfiledialogs.h"

int Editor::LeftMargin = Config::EDITOR_DEFAULT_LEFT_MARGIN;
int Editor::LineHeight = Config::EDITOR_DEFAULT_LINE_HEIGHT;
SDL_Point Editor::Cursor = { 0,0 };
std::vector<UILine*> Editor::lines = {};

std::vector<std::string> Editor::Format() {
    std::vector<std::string> text;
    for (UILine* l : Editor::lines)
        text.push_back(l->getText());
    return text;
}

Editor::Editor(const int w, const int h) {
    container = { 0, 0, w, h };
    cursor = { 0, 0, 1, LineHeight };

    selectionLength = 0;
    lineSelected = false;

    scrollPosition = 0;

    currentFile = "output/unknown.txt";

    fileSaved = false;
}

Editor::~Editor() {
    Config::Save();
}

#include <iostream>
void Editor::importData(const Data& d) {
    Theme::load(d.theme);

    if (d.lastfile.empty() || !fs::exists(d.lastfile)) {
        createNewFile(false);
        return;
    }

    LeftMargin = d.left_margin;
    LineHeight = d.line_height;
    Manager::SetFontSize(d.font_size);

    const std::vector<std::string> text = File::LoadTXT(d.lastfile);

    for (uint i = 0; i < text.size(); i++)
        lines.push_back(new UILine(text[i], i));
    
    Cursor = {d.cursor_x, d.cursor_y};
    scrollPosition = d.scroll_pos;

    updateCursorPlacement();

    fileSaved = true;
}

Data Editor::exportData() {
    return {
        .theme  = Theme::name,
        .lastfile   = currentFile,
        .cursor_x   = Cursor.x,
        .cursor_y   = Cursor.y,
        .scroll_pos = scrollPosition,
        .font_size  = Config::fontSize,
        .left_margin    = LeftMargin,
        .line_height    = LineHeight,
    };
}

void Editor::init() {
    Config::Load();
}

void Editor::update() {
    container.h = 0;
    for (auto l : lines)
        container.h += l->height();
        
    container.y = Window::ui->height() - scrollPosition * LineHeight * std::max(Config::FONT_DEFAULT_SIZE - Config::fontSize, 1);

    for (auto l : lines)
        l->update();
}

void Editor::render() {
    Manager::SetViewport(&container);

    lines[0]->draw();
    for (size_t i = 1; i < lines.size(); i++) {
        UILine* l = lines[i];
        int y = lines[i-1]->y() + lines[i-1]->height();
        l->drawAt(y);
    }

    Manager::DrawRect(&cursor, Theme::clr_cursor);

    if (selectionLength != 0) {
        const int length = abs(selectionLength);
        UILine* l = lines[Cursor.y];

        int x = Cursor.x;
        if (selectionLength > 0) x -= length;

        const std::string s = l->getText().substr(x, length);

        SDL_Rect selection = { cursor.x, cursor.y + 2, 0, cursor.h - 2 };

        Manager::SizeText(s, &selection.w, nullptr);
        if (selectionLength > 0) selection.x -= selection.w;

        Manager::DrawFilledRect(&selection, Theme::clr_selection);
    }

    Manager::SetViewport(nullptr);
}

void Editor::clear() {
    for (const auto& l : lines)
        l->destroy();
    lines.clear();
}

void Editor::destroy() {
    for (const auto& l : lines)
        l->destroy();
    lines.clear();
}

void Editor::reload() {
    for (const auto& l : lines)
        l->reload();
}

void Editor::place(const int x, const int y) {
    container.x = x;
    container.y = y;
}

void Editor::setWidth(const int w) {
    container.w = w;
    reload();
}

void Editor::updateCursorPlacement() {
    UILine* l = lines[Cursor.y];

    const std::string t = l->getText().substr(0, Cursor.x);
    Manager::SizeText(t, &cursor.x, nullptr);
    cursor.x += Editor::LeftMargin;

    cursor.y = l->y() + 2;
    cursor.h = Editor::LineHeight - 4;
}

void Editor::updateFontSize(int s) {
    Config::fontSize += s;
    if (Config::fontSize < Config::FONT_MIN_SIZE || Config::fontSize > Config::FONT_MAX_SIZE) {
        Config::fontSize -= s;
        return;
    }

    Manager::SetFontSize(Config::fontSize);

    LineHeight += s;

    reload();

    lines.back()->useAsMargin();

    updateCursorPlacement();
}

void Editor::resetFontSize() {
    Config::fontSize = Config::FONT_DEFAULT_SIZE;

    Manager::SetFontSize(Config::FONT_DEFAULT_SIZE);

    LineHeight = Config::EDITOR_DEFAULT_LINE_HEIGHT;
    LeftMargin = Config::EDITOR_DEFAULT_LEFT_MARGIN;

    reload();

    lines.back()->useAsMargin();

    updateCursorPlacement();
}

void Editor::insertChar(const char c) {
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
    if (Cursor.x >= (int)lines[Cursor.y]->size()) return;

    lines[Cursor.y]->erase(Cursor.x, 1);
}

/// TODO:
void Editor::deletePreviousWord() {

}

/// TODO:
void Editor::deleteNextWord() {

}

void Editor::select(const int d) {
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

        for (uint i = Cursor.y; i < lines.size(); i++) {
            lines[i]->setNumber(i);
        }

        moveCursorUp();
        l = lines[Cursor.y];
        Cursor.x = (int)l->size();
        updateCursorPlacement();

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

    const std::string text = l->getText();

    const std::string newLine = text.substr(Cursor.x);
    const std::string oldLine = text.substr(0, Cursor.x);

    l->setText(oldLine);

    UILine* nl = new UILine(newLine, Cursor.y + 1);
    lines.insert(lines.begin() + Cursor.y + 1, nl);

    for (uint i = nl->getNumber() + 1; i < lines.size(); i++) {
        UILine* l = lines[i];
        l->setNumber(i);
    }

    moveCursorDown();
    Cursor.x = 0;

    updateCursorPlacement();
}

bool Editor::deleteCurrentLine() {
    if (Cursor.x != 0 || Cursor.y <= 0) return false;

    UILine* l = lines[Cursor.y];
    const std::string oldLine = l->getText();

    l->destroy();
    lines.erase(lines.begin() + Cursor.y);

    for (uint i = Cursor.y; i < lines.size(); i++) {
        lines[i]->setNumber(i);
    }

    moveCursorUp();
    l = lines[Cursor.y];
    Cursor.x = (int)l->size();
    updateCursorPlacement();

    if (!oldLine.empty())
        l->append(oldLine);

    return true;
}

bool Editor::deleteNextLine() {
    UILine* l = lines[Cursor.y];

    if (Cursor.x != (int)l->size() || Cursor.y >= (int)lines.size() - 1)
        return false;

    UILine* lToDelete = lines[Cursor.y + 1];
    const std::string oldLine = lToDelete->getText();

    lToDelete->destroy();
    lines.erase(lines.begin() + Cursor.y + 1);

    if (!oldLine.empty())
        l->append(oldLine);

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
    Cursor.x = (int)lines[Cursor.y]->size();
    updateCursorPlacement();

    selectionLength = 0;
}

void Editor::moveCursorUp() {
    if (Cursor.y <= 0) return;

    selectionLength = 0;

    Cursor.y--;
    Cursor.x = std::min(Cursor.x, (int)lines[Cursor.y]->size());

    if (Cursor.y < scrollPosition)
        scroll(-1);

    updateCursorPlacement();
}

bool Editor::moveCursorDown() {
    if (Cursor.y >= (int)lines.size() - 1) return false;

    selectionLength = 0;

    Cursor.y++;
    Cursor.x = std::min(Cursor.x, (int)lines[Cursor.y]->size());

    if ((Cursor.y - scrollPosition + 2) * LineHeight > Window::screen.h - Window::ui->height())
        scroll(1);

    updateCursorPlacement();

    return true;
}

void Editor::moveCursorLeft() {
    if (Cursor.x == 0 && Cursor.y > 0) {
        moveCursorUp();
        Cursor.x = (int)lines[Cursor.y]->size();
    }
    else if (Cursor.x > 0)
        Cursor.x--;
    
    selectionLength = 0;

    updateCursorPlacement();
}

void Editor::moveCursorRight() {
    if (Cursor.x >= (int)lines[Cursor.y]->size() && Cursor.y < (int)lines.size() - 1) {
        moveCursorDown();
        Cursor.x = 0;
    }
    else if (Cursor.x < (int)lines[Cursor.y]->size())
        Cursor.x++;
    
    selectionLength = 0;
    updateCursorPlacement();
}

void Editor::scroll(int s) {
    scrollPosition += s;

    scrollPosition = std::max(0, std::min(scrollPosition, (int)lines.size() - 1));
}

void Editor::saveToCurrentFile() {
    if (fileSaved) {
        saveToNewFile();
        return;
    }

    bool success = File::Export(currentFile, Format());

    if (!success)
        tinyfd_messageBox("Ogmios", "Cannot save the file !", "ok", "error", 1);
}

void Editor::saveToNewFile() {
    char* path = tinyfd_saveFileDialog("Save", currentFile.c_str(), 0, NULL, NULL);

    fileSaved = File::Export(path, Format());
    if (fileSaved)
        currentFile = path;
    else
        tinyfd_messageBox("Ogmios", "Cannot save the file !", "ok", "error", 1);
}

void Editor::save(const bool isnew) {
    if (isnew) saveToNewFile();
    else saveToCurrentFile();
}

void Editor::load() {
    char* path = tinyfd_openFileDialog("Open", currentFile.c_str(), 4, File::FILTERS, NULL, 0);

    if (path != NULL) {
        lines.clear();

        std::vector<std::string> text = File::LoadTXT(path);

        for (unsigned int i = 0; i < text.size(); i++) {
            UILine* l = new UILine(text[i], i);
            lines.push_back(l);
        }

        jumpToFileEnd();

        fileSaved = true;
    }
    else {
        tinyfd_messageBox("Ogmios", "Cannot open the file !", "ok", "error", 1);
    }
}

void Editor::setClipboardText() {
    if (selectionLength == 0) {
        SDL_SetClipboardText("");
        return;
    }

    const int length = abs(selectionLength);
    UILine* l = lines[Cursor.y];

    int x = Cursor.x;
    if (selectionLength > 0) x -= length;

    const std::string s = l->getText().substr(x, length);

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

void Editor::createNewFile(bool checkIfSaved) {
    if (checkIfSaved && !fileSaved) {
        const int uinput = tinyfd_messageBox("Title", "File not saved!\nDo you want to save?", "yesnocancel", "warning", 0);

        if (uinput == 0)
            return;
        else if (uinput == 1)
            saveToCurrentFile();
    }

    clear();

    UILine* l = new UILine("", 0);
    lines.push_back(l);
    l->useAsMargin();
    jumpToFileStart();

    currentFile = "output/unknown.txt";
}
