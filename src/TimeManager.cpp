#include "TimeManager.h"
#include <iostream>

using namespace std;

TimeManager::TimeManager(SDL_Renderer *renderer)
    : renderer(renderer), font(nullptr), timeTexture(nullptr), musicTime(nullptr), musicGame(nullptr), Letgosound(nullptr), timeTexture1(nullptr), Time(300),
      timeOver(false), lastTime(SDL_GetTicks()), timeBegin(true), timeStart(120), musicTimePlaying(false), musicGamePlaying(false), waitingForSound(false), waitStart(0)
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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4048) < 0)
    {
        cerr << "LỖI SDL-MIX: " << Mix_GetError() << endl;
        writeLog("LỖI SDL-MIX: " + string(Mix_GetError()));
        return;
    }
    musicTime = Mix_LoadMUS("music/time.mp3");
    musicGame = Mix_LoadMUS("music/game.mp3");
    if (!musicTime || !musicGame)
    {
        cerr << "Lỗi Tải Nhạc Thoi Gian: " << Mix_GetError() << endl;
        writeLog("Lỗi Tải Nhạc Thoi Gian: " + string(Mix_GetError()));
    }
    else
        writeLog("Musictime loaded");

    Letgosound = Mix_LoadWAV("music/letgo.wav");
    if (!Letgosound)
    {
        cerr << "LỖI TẢI ÂM THANH letgo: " << Mix_GetError() << endl;
        writeLog("LỖI TẢI ÂM THANH letgo: " + string(Mix_GetError()));
    }
    else
    {
        Mix_VolumeChunk(Letgosound, 20);
        writeLog("ÂM THANH letgo TẢI THÀNH CÔNG");
    }
}

TimeManager::~TimeManager()
{
    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
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
    if (musicTime)
    {
        Mix_FreeMusic(musicTime);
        musicTime = nullptr;
        writeLog("MusicTime freed");
    }
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
}
void TimeManager::playMusicTime()
{

    if (musicTime && !Mix_PlayingMusic() && !musicTimePlaying)
    {
        musicTimePlaying = true;
        Mix_PlayMusic(musicTime, -1);
    }
}

void TimeManager::stopMusicTime()
{
    if (Mix_PlayingMusic() && musicTime && musicTimePlaying)
    {
        musicTimePlaying = false;
        Mix_HaltMusic();
        writeLog("MusicTime stopped");
    }
}
void TimeManager::playMusicGame()
{

    if (musicGame && !Mix_PlayingMusic() && !musicGamePlaying)
    {
        musicGamePlaying = true;
        Mix_PlayMusic(musicGame, -1);
    }
}

void TimeManager::stopMusicGame()
{
    if (Mix_PlayingMusic() && musicGame && musicGamePlaying)
    {
        musicGamePlaying = false;
        Mix_HaltMusic();
        writeLog("MusicTime stopped");
    }
}

void TimeManager::update()
{
    if (timeOver)
        return;

    Uint32 currentTime = SDL_GetTicks();

    if (waitingForSound)
    {
        if (currentTime - waitStart >= 1000)
        {
            waitingForSound = false;
        }
        return;
    }
    else
        playMusicGame();

    if (currentTime - lastTime >= 1200)
    {
        if (timeBegin)
        {
            if (timeStart > 0)
                timeStart--;
            else
            {
                timeBegin = false;
                waitingForSound = true;
                waitStart = currentTime;
                stopMusicTime();
                Mix_PlayChannel(-1, Letgosound, 0);
            }
        }

        if (!timeBegin && !waitingForSound)
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
    timeStart = 120;
    Time = 300;
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
    if (timeStart >= 120 * 0.1 && timeBegin || Time >= 300 * 0.1 && !timeBegin)
        textColor = {0, 191, 255, 255};
    else
    {
        textColor = {static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), 255};
        stopMusicGame();
        playMusicTime();
    }

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
void TimeManager::bxh(const char *filename)
{
    time_t now = time(0);
    ofstream file(filename, ios::app);
    if (!file.is_open())
    {
        cerr << "Không thể mở file: " << filename << endl;
        writeLog("Không thể mở file: " + string(filename));
        return;
    }
    file << setfill('0')
         << setw(2) << localtime(&now)->tm_mday << "/"
         << setw(2) << localtime(&now)->tm_mon + 1 << "/"
         << localtime(&now)->tm_year + 1900 << " "
         << setw(2) << localtime(&now)->tm_hour << ":"
         << setw(2) << localtime(&now)->tm_min << ":"
         << setw(2) << localtime(&now)->tm_sec << " "
         << 300 - Time << endl;
    file.close();
}
