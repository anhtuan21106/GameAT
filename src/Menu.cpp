#include "Menu.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

Menu::Menu(SDL_Renderer *renderer) : renderer(renderer), backgroundTexture(nullptr)
{
    SDL_Surface *surface = IMG_Load("Menu.png");
    {
        if (!surface)
        {
            cerr << "Không thể tải ảnh nền: " << IMG_GetError() << endl;
        }
        else
        {
            backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface); // chuyen surface thanh sdl_texture
            SDL_FreeSurface(surface);                                            // giai phong surface
            playButton = {740, 431, 416, 172};
            continueButton = {590, 616, 730, 160};
            exitButton = {778, 807, 380, 188};
        }
    }
}

Menu::~Menu()
{
    if (backgroundTexture)
        SDL_DestroyTexture(backgroundTexture);
}

void Menu::render()
{
    if (backgroundTexture)
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

MenuState Menu::handleEvents(SDL_Event &event)
{
}