#include "Character.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "Map.h"
using namespace std;
Character::Character(SDL_Renderer *renderer) : renderer(renderer), x(37 * 22), y(22 * 22), width(44), height(44), currentFrame(0)
{
    IMG_Init(IMG_INIT_PNG);
    const char *imagePaths[4] = {"image/character1.png", "image/character2.png", "image/character3.png", "image/character4.png"};
    for (int i = 0; i < 4; i++)
    {
        SDL_Surface *surface = IMG_Load(imagePaths[i]);
        if (!surface)
        {
            cerr << "Không thể tải ảnh: " << IMG_GetError() << endl;
            characterTextures[i] = nullptr;
        }
        else
        {
            characterTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
    }
}
Character::~Character()
{
    for (int i = 0; i < 4; i++)
    {
        if (characterTextures[i])
        {
            SDL_DestroyTexture(characterTextures[i]);
        }
    }
}
void Character::render()
{

    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(renderer, characterTextures[currentFrame], NULL, &rect);
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
            currentFrame = 1;
            break;
        case SDLK_DOWN:
            newY += 11;
            currentFrame = 0;
            break;
        case SDLK_LEFT:
            newX -= 11;
            currentFrame = 3;
            break;
        case SDLK_RIGHT:
            newX += 11;
            currentFrame = 2;
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
