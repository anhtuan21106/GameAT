#ifndef CHARACTER_H
#define CHARACTER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include "Map.h"
#include "Menu.h"
#include "log.h"
#include "TimeManager.h"
class Character
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *characterTextures[4];
    int x, y, width, height;
    int currentFrame;

public:
    Character(SDL_Renderer *renderer);
    ~Character();
    void render();
    void move(int newX, int newY, int newFrame, Map &map);
    void setCurrentPosition(int x, int y, int frame,int time,TimeManager &timeManager);
    void resetPosition();
    vector<int> getPrePosition(const char *filename,TimeManager &timeManager);
    void setPrePosition(const char *filename,TimeManager &timeManager);
};
#endif