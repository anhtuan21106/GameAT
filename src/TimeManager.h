#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "log.h"
class TimeManager
{
public:
    TimeManager(SDL_Renderer *renderer);
    ~TimeManager();
    void update();
    void render();
    bool isTimeUp();
    void setTimeUp(bool value);
    void setTimeUpStart(bool value);
    void resetTime();
    int getTimeStart();
    int getTime();
    void setTimeStart(int value);
    void setTime(int value);
    void rsLastTime();
private:
    SDL_Renderer *renderer;
    SDL_Texture *timeTexture;
    TTF_Font *font;
    SDL_Color textColor;
    SDL_Rect timeRect;
    Uint32 lastTime;
    int time;
    int timeStart;
    bool timeOver;
    bool timeBegin;
};
#endif
