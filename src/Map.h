#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "log.h"
#include <string>
#include <vector>
using namespace std;
enum TileType
{
    GRASS,
    WALL,
    TREASURE
};

class Map
{
private:
    SDL_Renderer *renderer;
    int tileSize;
    SDL_Texture *types[4];
    vector<vector<int>> tileMap;
    bool Gameover;
    bool ShowMap;

public:
    Map(SDL_Renderer *renderer, int tileSize);
    ~Map();
    bool LoadMap(const string &file);
    void render();
    int getTile(int x, int y, int width, int height);
    bool isGameover() const;
    void setGameover(bool value);
    void setShowMap(bool value);
};

#endif