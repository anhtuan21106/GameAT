#include "TimeManager.h"
#include <iostream>

using namespace std;

TimeManager::TimeManager(SDL_Renderer *renderer)
    : renderer(renderer), font(nullptr), timeTexture(NULL), timeTexture1(NULL), Time(120), timeOver(false), lastTime(SDL_GetTicks()), timeBegin(true), timeStart(90)
{
    font = TTF_OpenFont("ariblk.ttf", 20);
    if (!font)
    {
        cerr << "Không thể tải font: " << TTF_GetError() << endl;
        writeLog("Không thể tải font: " + string(TTF_GetError()));
    }
    else
    {
        timeRect = {30, 8, 100, 100};
        timeRect1 = {1800, 11, 100, 100};
        writeLog("TimeManager created");
    }
}

TimeManager::~TimeManager()
{
    if (font)
    {
        TTF_CloseFont(font);
        font == nullptr;
        writeLog("TimeManager destroyed");
    }
    if (timeTexture)
    {
        SDL_DestroyTexture(timeTexture);
        timeTexture = nullptr;
        writeLog("timeTexture deleted");
    }
    if (timeTexture1)
    {
        SDL_DestroyTexture(timeTexture1);
        timeTexture1 = nullptr;
        writeLog("timeTexture1 deleted");
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
            if (Time > 0)
                Time--;
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
bool TimeManager::isTimeUpStart()
{
    return timeBegin;
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
    return Time;
}
void TimeManager::setTimeStart(int value)
{
    timeStart = value;
}
void TimeManager::setTime(int value)
{
    Time = value;
}
void TimeManager::resetTime()
{
    timeStart = 90;
    Time = 120;
    writeLog("timeStart : " + to_string(timeStart) + " time : " + to_string(Time));
}
void TimeManager::render()
{
    time_t now = time(0);
    tm *localTime = localtime(&now);

    if (timeTexture1)
    {
        SDL_DestroyTexture(timeTexture1);
        timeTexture1 = nullptr;
    }
    SDL_Color textColor1 = {255, 20, 147, 255};
    stringstream timeStream1;
    timeStream1 << setw(2) << setfill('0') << localTime->tm_hour << ":"
                << setw(2) << setfill('0') << localTime->tm_min << ":"
                << setw(2) << setfill('0') << localTime->tm_sec;

    string timeString1 = timeStream1.str();
    SDL_Surface *textSurface1 = TTF_RenderText_Solid(font, timeString1.c_str(), textColor1);
    if (!textSurface1)
        return;
    timeTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
    timeRect1.w = textSurface1->w;
    timeRect1.h = textSurface1->h;
    SDL_FreeSurface(textSurface1);

    SDL_Rect timeRect2 = {timeRect1.x - 20, timeRect1.y - 5, timeRect1.w + 40, timeRect1.h + 10};
    SDL_SetRenderDrawColor(renderer, 0, 225, 0, 255);
    SDL_RenderDrawRect(renderer, &timeRect2);

    if (timeTexture1)
        SDL_RenderCopy(renderer, timeTexture1, NULL, &timeRect1);
}

void TimeManager::TimeGame()
{
    SDL_Color textColor;
    if (timeStart >= 90 * 0.15 && timeBegin || Time >= 120 * 0.15 && !timeBegin)
        textColor = {0, 191, 255, 255};
    else
        textColor = {static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), 255};

    if (timeTexture)
    {
        SDL_DestroyTexture(timeTexture);
        timeTexture = nullptr;
    }

    stringstream timeStream;
    timeStream << "Time" << (timeBegin ? " Start: " : ": ")
               << setw(2) << setfill('0') << (timeBegin ? timeStart / 60 : Time / 60) << ":"
               << setw(2) << setfill('0') << (timeBegin ? timeStart % 60 : Time % 60);

    string timeString = timeStream.str();
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, timeString.c_str(), textColor);
    if (!textSurface)
        return;
    timeTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    timeRect.w = textSurface->w;
    timeRect.h = textSurface->h;

    SDL_Rect timeRect3 = {timeRect.x - 10, timeRect.y - 5, timeRect.w + 20, timeRect.h + 9};
    SDL_SetRenderDrawColor(renderer, 0, 225, 0, 255);
    SDL_RenderDrawRect(renderer, &timeRect3);

    SDL_FreeSurface(textSurface);

    if (timeTexture)
        SDL_RenderCopy(renderer, timeTexture, NULL, &timeRect);
}
