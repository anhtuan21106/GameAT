#include "Character.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "Map.h"
#include <fstream>
#include "log.h"
using namespace std;
Character::Character(SDL_Renderer *renderer) : renderer(renderer), x(37 * 22), y(22 * 22), width(44), height(44), currentFrame(0)
{
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        cerr << "Lỗi khi khởi tạo SDL_Image: " << IMG_GetError() << endl;
        writeLog("Lỗi khi khởi tạo SDL_Image: " + string(IMG_GetError()));
        return;
    }

    const char *imagePaths[4] = {"image/character1.png", "image/character2.png", "image/character3.png", "image/character4.png"};
    for (int i = 0; i < 4; i++)
    {
        SDL_Surface *surface = IMG_Load(imagePaths[i]);
        if (!surface)
        {
            cerr << "Không thể tải ảnh: " << IMG_GetError() << endl;
            writeLog("Không thể tải ảnh [" + to_string(i) + "]: " + string(IMG_GetError()));
            characterTextures[i] = nullptr;
        }
        else
        {
            characterTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            if (!characterTextures[i])
            {
                cerr << "Không thể tạo texture: " << SDL_GetError() << endl;
                writeLog("Không thể tạo texture [" + to_string(i) + "]: " + string(SDL_GetError()));
            }
            else
            {
                writeLog("characterTextures[" + to_string(i) + "] created");
            }
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
            characterTextures[i] = nullptr;
            writeLog("characterTextures[" + to_string(i) + "] deleted");
        }
    }
    IMG_Quit();
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
    writeLog("x: " + to_string(x) + ", y: " + to_string(y) + ", currentFrame: " + to_string(currentFrame));
}

void Character::setCurrentPosition(int newX, int newY, int newFrame)
{
    x = newX;
    y = newY;
    currentFrame = newFrame;
    writeLog("setCurrentPosition x: " + to_string(x) + ", y: " + to_string(y) + ", currentFrame: " + to_string(currentFrame));
}
void Character::resetPosition()
{
    x = 37 * 22;
    y = 22 * 22;
    currentFrame = 0;
    writeLog("resetPosition x: " + to_string(x) + ", y: " + to_string(y) + ", currentFrame: " + to_string(currentFrame));
}
vector<int> Character::getPrePosition(const char *filename)
{
    vector<int> prePosition = {x, y, currentFrame};
    ifstream File(filename);
    if (!File.is_open())
    {
        cerr << "Không thể mở file: " << filename << endl;
        writeLog("Không thể mở file: " + string(filename));
        return prePosition;
    }
    File >> prePosition[0] >> prePosition[1] >> prePosition[2];
    writeLog("getPrePosition x: " + to_string(prePosition[0]) + ", y: " + to_string(prePosition[1]) + ", currentFrame: " + to_string(prePosition[2]));
    File.close();
    return prePosition;
}

void Character::setPrePosition(const char *filename)
{
    ofstream File(filename, ios::trunc);
    if (!File.is_open())
    {
        cerr << "Không thể mở file: " << filename << endl;
        writeLog("Không thể mở file: " + string(filename));
        return;
    }
    File << x << " " << y << " " << currentFrame;
    writeLog("setPrePosition x: " + to_string(x) + ", y: " + to_string(y) + ", currentFrame: " + to_string(currentFrame));
    File.close();
}
