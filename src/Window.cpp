#include "include/Window.h"

#include <iostream>
#include <map>

const std::string Window::Title = "Ogmios Editor";

bool Window::isRunning = false;
SDL_Renderer* Window::renderer = nullptr;
SDL_Rect Window::screen = { 0, 0, Config::WINDOW_DEFAULT_WIDTH, Config::WINDOW_DEFAULT_HEIGHT };
Event Window::event;

UI* Window::ui = nullptr;
Editor* Window::editor = nullptr;

void Window::Reload() {
    ui->reload();
    editor->reload();
}

Window::Window() : window(nullptr) {}

Window::~Window() {}

int Window::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() != 0) {
        std::cerr << SDL_GetError() << std::endl;
        return -2;
    }

    window = SDL_CreateWindow(
        Title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screen.w,
        screen.h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::cerr << "error creating the window" << std::endl;
        return -3;
    }
    SDL_SetWindowMinimumSize(window, Config::WINDOW_MIN_WIDTH, Config::WINDOW_MIN_HEIGHT);

    /// TODO: add app icon
    /*
    SDL_Surface* icon = IMG_Load("icons/Ogmios.png");
    SDL_SetWindowIcon(window, icon);
    */

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "error creating the renderer" << std::endl;
        return -4;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    event.linkTo(this);

    Manager::OpenFont("fonts/Oxanium-Regular.ttf");

    ui = new UI(screen.w, 30);
    editor = new Editor(screen.w, screen.h - ui->height());

    editor->init();
    ui->init();

    ui->place(0, 0);
    editor->place(0, ui->height());

    Manager::SetRenderDrawColor(Theme::clr_editor_background);

    SDL_StartTextInput();

    isRunning = true;

    return 0;
}

void Window::handleEvents() {
    SDL_PollEvent(&event.e);

    switch (event.e.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        event.handleKeyboardInputs();
        break;
    }
}

void Window::update() {
    ui->update();
    editor->update();
}

void Window::render() {
    SDL_RenderClear(renderer);

    editor->render();
    ui->render();

    SDL_RenderPresent(renderer);
}

void Window::kill() {
    Manager::CloseFont();

    ui->destroy();
    delete ui;
    ui = nullptr;

    editor->destroy();
    delete editor;
    editor = nullptr;

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_StopTextInput();
    SDL_Quit();
}

void Window::switchTheme() {
    //const std::map<std::string, std::string> next = {{"day", "night"}, {"night", "day"}};
    //Theme::load(next.at(Theme::name));

    Theme::load((std::map<std::string, std::string>){{"day", "night"}, {"night", "day"}}[Theme::name]);

    Reload();

    Manager::SetRenderDrawColor(Theme::clr_editor_background);
}

void Window::resize(int w, int h) {
    screen.w = w;
    screen.h = h;

    ui->setWidth(w);
    editor->setWidth(w);
}
