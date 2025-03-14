#ifndef MAP_H
#define MAP_H
#include <SDL2/SDL.h>
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
    vector<SDL_Texture *> types;
    vector<vector<int>> tileMap;
    bool Gameover;

public:
    Map(SDL_Renderer *renderer, int tileSize);
    ~Map();
    bool LoadMap(const string &file);
    void render();
    int getTile(int x, int y, int width, int height);
    bool isGameover() const;
    void setGameover(bool value);
};

#endif