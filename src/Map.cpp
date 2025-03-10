#include "Map.h"
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;
Map::Map(SDL_Renderer *renderer, int tileSize) : renderer(renderer), tileSize(tileSize)
{
    // IMG_Init(IMG_INIT_PNG);
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
    int rows = tileMap.size();
    int cols = tileMap[0].size();

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int tileType = tileMap[i][j];
            if (tileType < 0 || tileType >= types.size() || !types[tileType])
                continue;
            SDL_Rect dstRect = {tileSize * j, tileSize * i, tileSize, tileSize};
            SDL_RenderCopy(renderer, types[tileType], NULL, &dstRect);
        }
    }
}

bool Map::LoadMap(const string &file)
{
    ifstream File(file);
    if (!File.is_open())
    {
        cerr << "Không thể mở file bản đồ: " << file << endl;
        return false;
    }

    tileMap.clear();
    string line;
    while (getline(File, line))
    {
        vector<int> row;
        for (char c : line)
        {
            if (c == '0')
                row.push_back(GRASS);
            else if (c == '1')
                row.push_back(WALL);
            else if (c == '2')
                row.push_back(TREASURE);
            else continue;
        }
        tileMap.push_back(row);
    }
    File.close();
    return !tileMap.empty();
}
