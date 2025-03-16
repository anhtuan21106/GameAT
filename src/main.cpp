#include <SDL2/SDL.h>
#include <iostream>
#include "Menu.h"
#include "Map.h"
#include "Character.h"
#include "log.h"
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
        writeLog("Lỗi khởi tạo SDL: " + string(SDL_GetError()));
        exit(1);
    }
    window = SDL_CreateWindow("JOURNEY IN THE DARK", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        cerr << "Lỗi tạo cửa sổ: " << SDL_GetError() << endl;
        writeLog("Lỗi tạo cửa sổ: " + string(SDL_GetError()));
        SDL_Quit();
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cerr << "Lỗi tạo renderer: " << SDL_GetError() << endl;
        writeLog("Lỗi tạo renderer: " + string(SDL_GetError()));
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    writeLog("START GAME");
}

void cleanup()
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    writeLog("END GAME");
}

int main(int argc, char *argv[])
{
    init();
    bool running = true;
    SDL_Event event;
    Menu menu(renderer);
    MenuState state = MENU;
    Map map(renderer, 22);
    Character character(renderer);
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || state == EXIT)
            {
                character.setPrePosition("prePosition.txt");
                running = false;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_k)
            {
                if (menu.isMusicPlaying())
                    menu.stopMusic();
                else
                    menu.playMusic();
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
            {
                character.setPrePosition("prePosition.txt");
                state = MENU;
            }

            if ((state == PLAY || state == CONTINUE) && (event.type == SDL_KEYDOWN))
            {
                int stepX = 0, stepY = 0, currentFrame = 0;
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    stepY = -11;
                    currentFrame = 1;
                    break;
                case SDLK_DOWN:
                    stepY = 11;
                    currentFrame = 0;
                    break;
                case SDLK_LEFT:
                    stepX = -11;
                    currentFrame = 3;
                    break;
                case SDLK_RIGHT:
                    stepX = 11;
                    currentFrame = 2;
                    break;
                }
                character.move(stepX, stepY, currentFrame, map);
            }
            else if (state == MENU)
            {
                if (!menu.isMusicPlaying())
                    menu.playMusic();
                state = menu.handleEvents(event);
                if ((state == PLAY || state == CONTINUE) && !map.LoadMap("map.txt"))
                {
                    cerr << "Lỗi tạo map" << endl;
                    writeLog("Lỗi tạo map");
                    cleanup();
                    return -1;
                }

                if (state == PLAY)
                    character.resetPosition();
                else if (state == CONTINUE)
                {
                    pair<int, int> prePosition = character.getPrePosition("prePosition.txt");
                    if (prePosition.first != -1 && prePosition.second != -1)
                    {
                        character.setCurrentPosition(prePosition.first, prePosition.second);
                    }
                }
            }
        }
        SDL_RenderClear(renderer);
        if (map.isGameover())
        {
            map.setGameover(false);
            character.resetPosition();
            character.setPrePosition("prePosition.txt");
            menu.winGameMusic();
            SDL_Delay(1000);
            state = MENU;
        }
        if ((state == PLAY) || (state == CONTINUE))
        {
            map.render();
            character.render();
        }
        else
        {
            menu.render();
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    cleanup();
    return 0;
}
