#include "TimeManager.h"
#include <iostream>

using namespace std;

TimeManager::TimeManager(SDL_Renderer *renderer)
    : renderer(renderer), timeTexture(NULL), time(10), timeOver(false), lastTime(SDL_GetTicks()), timeBegin(true), timeStart(3)
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
        if (timeBegin)
        {
            if (timeStart > 0)
                timeStart--;
            else
            {
                timeBegin = false;
                writeLog("Countdown timeout");
            }
        }

        if (!timeBegin)
        {
            if (time > 0)
                time--;
            else
                timeOver = true;
        }
        lastTime = currentTime;
    }
}
void TimeManager::rsLastTime()
{
    lastTime = SDL_GetTicks();
}
bool TimeManager::isTimeUp()
{
    return timeOver;
}
void TimeManager::setTimeUp(bool value)
{
    timeOver = value;
}

void TimeManager::setTimeUpStart(bool value)
{
    timeBegin = value;
}
int TimeManager::getTimeStart()
{
    return timeStart;
}

int TimeManager::getTime()
{
    return time;
}
void TimeManager::setTimeStart(int value)
{
    timeStart = value;
}
void TimeManager::setTime(int value)
{
    time = value;
}
void TimeManager::resetTime()
{
    timeStart = 3;
    time = 10;
    writeLog("timeStart : " + to_string(timeStart) + " time : " + to_string(time));
}
void TimeManager::render()
{
    if (timeTexture)
    {
        SDL_DestroyTexture(timeTexture);
        timeTexture = nullptr;
    }

    string timeString;
    if (timeBegin)
    {
        timeString = "TimeStart: " + to_string(timeStart / 60) + ":" + to_string(timeStart % 60);
    }
    else
    {
        timeString = "Time: " + to_string(time / 60) + ":" + to_string(time % 60);
    }
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
