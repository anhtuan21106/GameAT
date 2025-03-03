#ifndef MENU_H
#define MENU_H
#include <SDL2/SDL.h>
enum MenuState
{
    PLAY,
    CONTINUE,
    EXIT
};
class Menu
{
private:
    SDL_Renderer *renderer;
    SDL_Texture *backgroundTexture;
    SDL_Rect playButton;
    SDL_Rect continueButton;
    SDL_Rect exitButton;

public:
    Menu(SDL_Renderer *renderer);
    ~Menu();
    void render();                            // vẽ menu
    MenuState handleEvents(SDL_Event &event); // xử lý sự kiện
};
#endif