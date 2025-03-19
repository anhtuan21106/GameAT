#include <SDL2/SDL.h>
#include <iostream>
#include "Menu.h"
#include "Map.h"
#include "Character.h"
#include "log.h"
#include "TimeManager.h"
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

    if (TTF_Init() == -1)
    {
        cerr << "LỖI KHỞI TẠO SDL_ttf: " << TTF_GetError() << endl;
        writeLog("LỖI KHỞI TẠO SDL_ttf: " + string(TTF_GetError()));
        SDL_Quit();
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
    TTF_Quit();
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
    TimeManager timeManager(renderer);
    bool sdlk_k = false;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || state == EXIT)
            {
                character.setPrePosition("prePosition.txt");
                running = false;
                writeLog("EXIT GAME");
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_k)
            {
                sdlk_k = true;
                if (menu.isMusicPlaying())
                {
                    menu.stopMusic();
                    writeLog("STOP MUSIC");
                }
                else
                {
                    menu.playMusic();
                    writeLog("PLAY MUSIC");
                }
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
            {
                character.setPrePosition("prePosition.txt");
                state = MENU;
                writeLog("BACK TO MENU");
            }

            if ((state == PLAY || state == CONTINUE) && (event.type == SDL_KEYDOWN))
            {
                int stepX = 0, stepY = 0, currentFrame = -1;
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
                if (currentFrame != -1)
                    character.move(stepX, stepY, currentFrame, map);
            }
            else if (state == MENU)
            {
                if (!menu.isMusicPlaying() && !sdlk_k)
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
                {
                    writeLog("PLAY GAME");
                    timeManager.resetTime();
                    character.resetPosition();
                }
                else if (state == CONTINUE)
                {
                    writeLog("CONTINUE GAME");
                    vector<int> prePosition = character.getPrePosition("prePosition.txt");
                    if (!prePosition.empty())
                    {
                        character.setCurrentPosition(prePosition[0], prePosition[1], prePosition[2]);
                    }
                }
            }
        }
        SDL_RenderClear(renderer);
        if (map.isGameover())
        {
            timeManager.resetTime();
            writeLog("WIN GAME");
            map.setGameover(false);
            character.resetPosition();
            character.setPrePosition("prePosition.txt");
            menu.winGameMusic();
            SDL_Delay(1000);
            state = MENU;
        }
        if (timeManager.isTimeUp())
        {
            timeManager.resetTime();
            timeManager.setTimeUp(false);
            character.resetPosition();
            character.setPrePosition("prePosition.txt");
            state = MENU;
        }
        if ((state == PLAY) || (state == CONTINUE))
        {
            sdlk_k = false;
            timeManager.update();
            map.render();
            character.render();
            timeManager.render();
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
