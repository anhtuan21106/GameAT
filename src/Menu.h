#ifndef MENU_H
#define MENU_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
enum MenuState
{
    PLAY,
    CONTINUE,
    EXIT,
    NONE
};
class Menu
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *backgroundTexture;
    Mix_Music *backgroundMusic;
    Mix_Chunk *buttonSound;
    SDL_Rect playButton;
    SDL_Rect continueButton;
    SDL_Rect exitButton;

public:
    Menu(SDL_Renderer *renderer);
    ~Menu();
    void render();                            // vẽ menu
    MenuState handleEvents(SDL_Event &event); // xử lý sự kiện
    void playMusic();
    void playMusicButton();
    void stopMusic();
};
#endif