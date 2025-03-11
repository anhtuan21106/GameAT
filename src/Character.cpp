#include "Character.h"
#include <SDL2/SDL.h>
Character::Character(SDL_Renderer *renderer) : renderer(renderer), x(0), y(0), width(22), height(22) {}
Character::~Character()
{
    SDL_Quit();
}
void Character::render()
{
    SDL_Rect rect = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
void Character::move(const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
            y -= 5;
            break;
        case SDLK_DOWN:
            y += 5;
            break;
        case SDLK_LEFT:
            x -= 5;
            break;
        case SDLK_RIGHT:
            x += 5;
            break;
        }
    }
    if(x < 0)
        x = 0;
    if(y < 0)
        y = 0;
    if(x + width > 1920)
        x = 1920 - width;
    if(y + height > 1080)
        y = 1080 - height;
}
