#include "Character.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "Map.h"
#include <fstream>
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

void Character::move(int stepX, int stepY, int newFrame, Map &map)
{
    currentFrame = newFrame;

    int tileType = map.getTile(x + stepX, y + stepY, width, height);
    if (tileType == 0)
    {
        x += stepX;
        y += stepY;
    }
    else if (tileType == 2)
    {
        SDL_Delay(500);
    }
}
pair<int, int> Character::getCurrentPosition()
{
    return {x, y};
}
void Character::setCurrentPosition(int newX, int newY)
{
    x = newX;
    y = newY;
}
void Character::resetPosition()
{
    x = 37 * 22;
    y = 22 * 22;
}
pair<int, int> Character::getPrePosition(const char *filename)
{
    pair<int, int> prePosition = {-1, -1};
    ifstream File(filename);
    if (!File.is_open())
    {
        cerr << "Không thể mở file: " << filename << endl;
        return prePosition;
    }
    File >> prePosition.first >> prePosition.second;
    File.close();
    return prePosition;
}

void Character::setPrePosition(const char *filename)
{
    ofstream File(filename);
    ofstream clearFile(filename, ios::trunc);
    clearFile.close();
    if (!File.is_open())
    {
        cerr << "Không thể mở file : " << filename << endl;
        return;
    }
    File << x << " " << y;
    File.close();
    return;
};