#ifndef MAP_H
#define MAP_H
#include <SDL2/SDL.h>
#include<string>
#include<vector>
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
    SDL_Texture *map;
    int tileSize;
    bool ShowMap;
    vector<SDL_Texture *> types;
public:
    Map(SDL_Renderer *renderer, int tileSize);
    ~Map();
    bool LoadMap(const string &file); 

    void render();
};

#endif