#include "Map.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;
Map::Map(SDL_Renderer *renderer, int tileSize) : renderer(renderer), tileSize(tileSize), map(nullptr), ShowMap(true)
{
    IMG_Init(IMG_INIT_PNG);
    types.resize(3, nullptr);
    string fileNames[] = {"grass.png", "wall.png", "treasure.png"};
    for (int i = 0; i < 3; i++)
    {
        SDL_Surface *surface = IMG_Load(fileNames[i].c_str());
        if (!surface)
        {
            cerr << "Không thể tải ảnh: " << IMG_GetError() << endl;
        }
        types[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!types[i])
        {
            cerr << "Không thể tạo texture: " << SDL_GetError() << endl;
        }
    }
}

Map::~Map()
{
    for (int i = 0; i < 3; i++)
    {
        if (types[i])
            SDL_DestroyTexture(types[i]);
    }
    IMG_Quit();
}

void Map::render()
{
}
bool Map::LoadMap(const string &file)
{
    
}