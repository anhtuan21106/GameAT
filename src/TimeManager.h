#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "log.h"
#include <iomanip>
#include <ctime>
class TimeManager
{
public:
    TimeManager(SDL_Renderer *renderer);
    ~TimeManager();
    void update();
    void render();
    bool isTimeUp();
    bool isTimeUpStart();
    void setTimeUp(bool value);
    void setTimeUpStart(bool value);
    void resetTime();
    int getTimeStart();
    int getTime();
    void setTimeStart(int value);
    void setTime(int value);
    void rsLastTime();
    void TimeGame();

private:
    SDL_Renderer *renderer;
    SDL_Texture *timeTexture;
    SDL_Texture *timeTexture1;
    SDL_Rect timeRect;
    SDL_Rect timeRect1;
    TTF_Font *font;
    Uint32 lastTime;
    int Time;
    int timeStart;
    bool timeOver;
    bool timeBegin;
};
#endif
