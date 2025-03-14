#include "Character.h"
#include <SDL2/SDL.h>
#include "Map.h"
Character::Character(SDL_Renderer *renderer) : renderer(renderer), x(37 * 22), y(22 * 22), width(22), height(22) {}
Character::~Character()
{
}
void Character::render()
{
    SDL_Rect rect = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Character::move(const SDL_Event &event, Map &map)
{
    int newX = x, newY = y;

    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
            newY -= 11;
            break;
        case SDLK_DOWN:
            newY += 11;
            break;
        case SDLK_LEFT:
            newX -= 11;
            break;
        case SDLK_RIGHT:
            newX += 11;
            break;
        }
    }
    int tileType = map.getTile(newX, newY, width, height);
    if (tileType == 0)
    {
        x = newX;
        y = newY;
    }
    else if (tileType == 2)
    {
        SDL_Delay(500);
    }
}
void Character::resetPosition()
{
    x = 37 * 22;
    y = 22 * 22;
}
