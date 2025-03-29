#ifndef MENU_H
#define MENU_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "log.h"
enum MenuState
{
    PLAY,
    CONTINUE,
    EXIT,
    MENU
};
class Menu
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *backgroundTexture;
    Mix_Music *backgroundMusic;
    Mix_Chunk *buttonSound;
    Mix_Chunk *winGame;
    Mix_Chunk *loseGame;
    SDL_Rect playButton;
    SDL_Rect continueButton;
    SDL_Rect exitButton;
    SDL_Rect *effectButton;

public:
    Menu(SDL_Renderer *renderer);
    ~Menu();
    void render();
    MenuState handleEvents(SDL_Event &event);
    void playMusic();
    void playMusicButton();
    void winGameMusic();
    void loseGameMusic();
    void stopMusic();
};
#endif