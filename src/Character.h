#ifndef CHARACTER_H
#define CHARACTER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Map.h"
#include "Menu.h"
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
    pair<int, int> getCurrentPosition();
    void setCurrentPosition(int x, int y);
    void resetPosition();
    pair<int, int> getPrePosition(const char *filename);
    void setPrePosition(const char *filename);
};
#endif