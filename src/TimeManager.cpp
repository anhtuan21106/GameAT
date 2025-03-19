#include "TimeManager.h"
#include <iostream>

using namespace std;

TimeManager::TimeManager(SDL_Renderer *renderer)
    : renderer(renderer), timeTexture(NULL), time(10), timeOver(false),lastTime(SDL_GetTicks())
{
    font = TTF_OpenFont("ariblk.ttf", 20);
    if (!font)
    {
        cerr << "Không thể tải font: " << TTF_GetError() << endl;
        writeLog("Không thể tải font: " + string(TTF_GetError()));
    }
    else
    {
        textColor = {255, 0, 0, 255};
        timeRect = {30, 11, 100, 100};
        writeLog("TimeManager created");
    }
}

TimeManager::~TimeManager()
{
    if (font)
    {
        TTF_CloseFont(font);
        writeLog("TimeManager destroyed");
    }
    if (timeTexture)
    {
        SDL_DestroyTexture(timeTexture);
        timeTexture = nullptr;
        writeLog("timeTexture deleted");
    }
}
void TimeManager::update()
{
    if (timeOver)
        return;
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - lastTime >= 1000)
    {
        if (time > 0)
            time--;
        else
            timeOver = true;
        lastTime = currentTime;
    }
}

bool TimeManager::isTimeUp()
{
    return timeOver;
}

void TimeManager::setTimeUp(bool value)
{
    timeOver = value;
}


void TimeManager::resetTime(){
    time = 10;
    lastTime = SDL_GetTicks();
}
void TimeManager::render()
{
    if (timeTexture)
    {
        SDL_DestroyTexture(timeTexture);
        timeTexture = nullptr;
    }

    string timeString = "Time: " + to_string(time / 60) + ":" + to_string(time % 60);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, timeString.c_str(), textColor);
    if (!textSurface)
        return;
    timeTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    timeRect.w = textSurface->w;
    timeRect.h = textSurface->h;
    SDL_FreeSurface(textSurface);

    if (timeTexture)
    {
        SDL_RenderCopy(renderer, timeTexture, NULL, &timeRect);
    }
}
