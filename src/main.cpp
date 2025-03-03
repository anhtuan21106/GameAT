#include <SDL2/SDL.h>
#include <iostream>
#include "Menu.h"
using namespace std;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cerr << "Lỗi khởi tạo SDL: " << SDL_GetError() << endl;
        exit(1);
    }
    window = SDL_CreateWindow("TIM KHO BAU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        cerr << "Lỗi tạo cửa sổ: " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cerr << "Lỗi tạo renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

void cleanup()
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    init();
    bool running = true;
    SDL_Event event;
    Menu menu(renderer);

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                running = false;
            }
        }
        SDL_RenderClear(renderer);
        menu.render();
    }

    cleanup();
    return 0;
}
