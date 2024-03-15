#include "include/Window.h"

Window w;

int main(int argc, char const *argv[]) {
    if (w.init() < 0) exit(EXIT_FAILURE);

    while (Window::isRunning) {
        w.handleEvents();
        w.update();
        w.render();
    }

    w.kill();

    exit(EXIT_SUCCESS);
}
