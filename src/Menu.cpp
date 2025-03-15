#include "Menu.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

Menu::Menu(SDL_Renderer *renderer) : renderer(renderer), backgroundTexture(nullptr), backgroundMusic(nullptr), buttonSound(nullptr), winGame(nullptr)
{
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface *surface = IMG_Load("image/Menu.png");
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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cerr << "Lỗi sdl-mix: " << Mix_GetError() << endl;
    }
    else
    {
        backgroundMusic = Mix_LoadMUS("music/menu.mp3");
        if (!backgroundMusic)
        {
            cerr << "lỗi tải nhạc nền: " << Mix_GetError() << endl;
        }
        else
        {
            playMusic();
        }
    }
    buttonSound = Mix_LoadWAV("music/button.wav");
    winGame = Mix_LoadWAV("music/win.wav");
    if (!buttonSound)
    {
        cerr << "lỗi tải âm thanh nút ấn: " << Mix_GetError() << endl;
    }
    else
    {
        Mix_VolumeChunk(buttonSound, 20);
    }
    if (!winGame)
    {
        cerr << "Lỗi tải âm thanh win: " << Mix_GetError() << endl;
    }
    else
    {
        Mix_VolumeChunk(winGame, 60);
    }
}

Menu::~Menu()
{
    if (backgroundTexture)
        SDL_DestroyTexture(backgroundTexture);
    if (backgroundMusic)
        Mix_FreeMusic(backgroundMusic);
    if (buttonSound)
        Mix_FreeChunk(buttonSound);
    if (winGame)
        Mix_FreeChunk(winGame);

    Mix_CloseAudio();
    Mix_Quit();
}

void Menu::render()
{
    if (renderer && backgroundTexture)
    {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }
}

MenuState Menu::handleEvents(SDL_Event &event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        int x = event.button.x;
        int y = event.button.y;

        if (x >= playButton.x && x <= playButton.x + playButton.w &&
            y >= playButton.y && y <= playButton.y + playButton.h)
        {
            playMusicButton();
            stopMusic();
            return PLAY;
        }

        if (x >= continueButton.x && x <= continueButton.x + continueButton.w &&
            y >= continueButton.y && y <= continueButton.y + continueButton.h)
        {
            playMusicButton();
            stopMusic();
            return CONTINUE;
        }

        if (x >= exitButton.x && x <= exitButton.x + exitButton.w &&
            y >= exitButton.y && y <= exitButton.y + exitButton.h)
        {
            playMusicButton();
            stopMusic();
            return EXIT;
        }
    }
    return MENU;
}

void Menu::playMusic()
{
    if (backgroundMusic)
    {
        Mix_PlayMusic(backgroundMusic, -1); // lặp vô hạn
    }
}

void Menu::playMusicButton()
{
    if (buttonSound)
    {
        Mix_PlayChannel(-1, buttonSound, 0); // lặp 1 lần
    }
}
void Menu::winGameMusic()
{
    if (winGame)
    {
        Mix_PlayChannel(-1, winGame, 0); // lặp 1 lần
    }
}
void Menu::stopMusic()
{
    Mix_HaltMusic();
}
bool Menu::isMusicPlaying()
{
    return Mix_PlayingMusic();
}