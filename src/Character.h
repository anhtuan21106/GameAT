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
    int x, y, width, height;

public:
    Character(SDL_Renderer *renderer);
    ~Character();
    void render();
    void move(const SDL_Event &event, Map &map);
    void resetPosition();
};
#endif