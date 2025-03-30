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
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

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
    bool musicPlaying = true;
    Uint32 waitStart = 0;
    bool waiting = false;
    int pressCount = 0;
    bool first = true;
    bool end = true;
    bool over = true;
    while (running)
    {
        Uint32 currentTime = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || state == EXIT)
            {
                character.setPrePosition("prePosition.txt", timeManager);
                running = false;
                writeLog("EXIT GAME");
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_k && state == MENU)
            {
                musicPlaying = !musicPlaying;
                musicPlaying ? menu.playMusic() : menu.stopMusic();
                writeLog(musicPlaying ? "PLAY MUSIC" : "STOP MUSIC");
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
            {
                character.setPrePosition("prePosition.txt", timeManager);
                state = MENU;
                writeLog("BACK TO MENU");
            }

            if ((state == PLAY || state == CONTINUE) && event.type == SDL_KEYDOWN && !timeManager.isTimeUpStart())
            {
                int stepX = 0, stepY = 0, currentFrame = -1;
                pressCount = pressCount % 3;
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    stepY = -11;
                    if (pressCount == 0)
                        currentFrame = 1;
                    else if (pressCount == 1)
                        currentFrame = 8;
                    else
                        currentFrame = 9;
                    pressCount++;
                    break;
                case SDLK_DOWN:
                    stepY = 11;
                    if (pressCount == 0)
                        currentFrame = 0;
                    else if (pressCount == 1)
                        currentFrame = 4;
                    else
                        currentFrame = 5;
                    pressCount++;
                    break;
                case SDLK_LEFT:
                    stepX = -11;
                    if (pressCount == 0)
                        currentFrame = 3;
                    else if (pressCount == 1)
                        currentFrame = 6;
                    else
                        currentFrame = 7;
                    pressCount++;
                    break;
                case SDLK_RIGHT:
                    stepX = 11;
                    if (pressCount == 0)
                        currentFrame = 2;
                    else if (pressCount == 1)
                        currentFrame = 10;
                    else
                        currentFrame = 11;
                    pressCount++;
                    break;
                }
                if (currentFrame != -1)
                    character.move(stepX, stepY, currentFrame, map);
            }
            else if (state == MENU)
            {
                if (musicPlaying)
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
                    timeManager.rsLastTime();
                    writeLog("PLAY GAME");
                    timeManager.setTimeUpStart(true);
                    timeManager.resetTime();
                    character.resetPosition();
                    first = true;
                }
                else if (state == CONTINUE)
                {
                    first = true;
                    timeManager.rsLastTime();
                    writeLog("CONTINUE GAME");
                    vector<int> prePosition = character.getPrePosition("prePosition.txt", timeManager);
                    character.setCurrentPosition(prePosition[0], prePosition[1], prePosition[2], prePosition[3], prePosition[4], timeManager);
                    if (timeManager.getTimeStart() <= 0)
                        timeManager.setTimeUpStart(false);
                    else
                    {
                        timeManager.setTimeUpStart(true);
                        timeManager.setTime(300);
                    }
                }
            }
        }
        SDL_RenderClear(renderer);

        if (!timeManager.isTimeUpStart())
        {
            if (first)
            {
                map.setShowMap(false);
                first = false;
                end = true;
            }
            if (timeManager.getTime() % 10 == 0 && timeManager.getTime() <= 100 && end)
            {
                map.setShowMap(true);
                end = false;
            }
            else if (currentTime - waitStart >= 1500)
            {
                waitStart = currentTime;
                map.setShowMap(false);
                end = true;
            }
        }
        else
            map.setShowMap(true);

        if (waiting && currentTime - waitStart >= 2000)
        {
            waiting = false;
            map.setGameover(false);
            timeManager.setTimeUp(false);
            state = MENU;
            over = true;
        }
        else if ((map.isGameover() || timeManager.isTimeUp()) && over)
        {
            timeManager.stopMusicGame();
            timeManager.stopMusicTime();
            if (map.isGameover())
            {
                timeManager.bxh("bxh.txt");
                writeLog("WIN GAME");
                timeManager.setTimeUp(true);
                menu.winGameMusic();
            }
            else if (timeManager.isTimeUp())
            {
                writeLog("LOSE GAME");
                map.setGameover(true);
                menu.loseGameMusic();
            }
            waiting = true;
            over = false;
            waitStart = currentTime;
            timeManager.resetTime();
            timeManager.setTimeUpStart(true);
            character.resetPosition();
            character.setPrePosition("prePosition.txt", timeManager);
        }
        if ((state == PLAY) || (state == CONTINUE))
        {
            map.render();
            if (!map.isGameover())
                character.render();
            timeManager.TimeGame();
            timeManager.update();
        }
        else
        {
            timeManager.stopMusicTime();
            timeManager.stopMusicGame();
            menu.render();
        }
        timeManager.render();

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    cleanup();
    return 0;
}
