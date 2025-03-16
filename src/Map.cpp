#include "Map.h"
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;
Map::Map(SDL_Renderer *renderer, int tileSize) : renderer(renderer), tileSize(tileSize), Gameover(false)
{
    IMG_Init(IMG_INIT_PNG);
    types.resize(3, nullptr);
    const char *fileNames[] = {"image/grass1.png", "image/wall1.png", "image/treasure.png"};
    for (int i = 0; i < 3; i++)
    {
        SDL_Surface *surface = IMG_Load(fileNames[i]);
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

bool Map::LoadMap(const string &file)
{
    Gameover = false;
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
            else
                continue;
        }
        tileMap.push_back(row);
    }
    File.close();
    return !tileMap.empty();
}
bool Map::isGameover() const
{
    return Gameover;
}

int Map::getTile(int x, int y, int width, int height)
{
    int leftCol = x / tileSize;
    int rightCol = (x + width - 1) / tileSize;
    int topRow = y / tileSize;
    int bottomRow = (y + height - 1) / tileSize;
    for (int row = topRow; row <= bottomRow; row++)
    {
        for (int col = leftCol; col <= rightCol; col++)
        {
            if (tileMap[row][col] == WALL)
                return 1;
            if (tileMap[row][col] == TREASURE)
            {
                Gameover = true;
                return 2;
            }
        }
    }
    return 0;
}

void Map::render()
{
    if (Gameover)
    {
        return;
    }
    else
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
}
void Map::setGameover(bool value)
{
    Gameover = value;
}