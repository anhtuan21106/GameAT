#include "Menu.h"
using namespace std;

Menu::Menu(SDL_Renderer *renderer) : renderer(renderer), backgroundTexture(nullptr), backgroundMusic(nullptr), buttonSound(nullptr), winGame(nullptr), loseGame(nullptr), effectButton(nullptr)
{
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        cerr << "LỖI KHI KHỞI TẠO SDL_Image: " << IMG_GetError() << endl;
        writeLog("LỖI KHI KHỞI TẠO SDL_Image: " + string(IMG_GetError()));
        return;
    }
    SDL_Surface *surface = IMG_Load("image/Menu.png");
    if (!surface)
    {
        cerr << "KHÔNG THỂ TẢI ẢNH MENU: " << IMG_GetError() << endl;
        writeLog("KHÔNG THỂ TẢI ẢNH MENU: " + string(IMG_GetError()));
        backgroundTexture = nullptr;
    }
    else
    {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!backgroundTexture)
        {
            cerr << "KHÔNG THỂ TẠO TEXTURE CHO MENU: " << SDL_GetError() << endl;
            writeLog("KHÔNG THỂ TẠO TEXTURE CHO MENU: " + string(SDL_GetError()));
        }
        else
        {
            writeLog("BACKGROUND MENU TEXTURE TẠO THÀNH CÔNG");
        }
    }

    playButton = {740, 431, 416, 172};
    continueButton = {590, 616, 730, 160};
    exitButton = {778, 807, 380, 188};

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cerr << "LỖI SDL-MIX: " << Mix_GetError() << endl;
        writeLog("LỖI SDL-MIX: " + string(Mix_GetError()));
        return;
    }
    backgroundMusic = Mix_LoadMUS("music/menu.mp3");
    if (!backgroundMusic)
    {
        cerr << "LỖI TẢI NHẠC NỀN: " << Mix_GetError() << endl;
        writeLog("LỖI TẢI NHẠC NỀN: " + string(Mix_GetError()));
    }
    else
    {
        writeLog("NHẠC NỀN MENU TẢI THÀNH CÔNG");
    }
    buttonSound = Mix_LoadWAV("music/button.wav");
    if (!buttonSound)
    {
        cerr << "LỖI TẢI ÂM THANH NÚT ẤN: " << Mix_GetError() << endl;
        writeLog("LỖI TẢI ÂM THANH NÚT ẤN: " + string(Mix_GetError()));
    }
    else
    {
        Mix_VolumeChunk(buttonSound, 20);
        writeLog("ÂM THANH NÚT ẤN TẢI THÀNH CÔNG");
    }

    winGame = Mix_LoadWAV("music/win.wav");
    loseGame = Mix_LoadWAV("music/lose.wav");
    if (!winGame)
    {
        cerr << "LỖI TẢI ÂM THANH WIN: " << Mix_GetError() << endl;
        writeLog("LỖI TẢI ÂM THANH WIN: " + string(Mix_GetError()));
    }
    else
    {
        Mix_VolumeChunk(winGame, 80);
        writeLog("ÂM THANH WIN TẢI THÀNH CÔNG");
    }

    if (!loseGame)
    {
        cerr << "LỖI TẢIÂM THANH LOSE: " << Mix_GetError() << endl;
        writeLog("LỖI TẢI ÂM THANH LOSE: " + string(Mix_GetError()));
    }
    else
    {
        Mix_VolumeChunk(loseGame, 80);
        writeLog("ÂM THANH LOSE TẢI THÀNH CÔNG");
    }
}

Menu::~Menu()
{
    if (backgroundTexture)
    {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
        writeLog("TEXTURE MENU ĐÃ ĐƯỢC GIẢI PHÓNG");
    }
    if (backgroundMusic)
    {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
        writeLog("NHẠC NỀN MENU ĐÃ ĐƯỢC GIẢI PHÓNG");
    }
    if (buttonSound)
    {
        Mix_FreeChunk(buttonSound);
        buttonSound = nullptr;
        writeLog("ÂM THANH NÚT ẤN ĐÃ ĐƯỢC GIẢI PHÓNG");
    }
    if (winGame)
    {
        Mix_FreeChunk(winGame);
        winGame = nullptr;
        writeLog("ÂM THANH WIN ĐÃ ĐƯỢC GIẢI PHÓNG");
    }

    Mix_CloseAudio();
    writeLog("MIXER AUDIO ĐÃ ĐÓNG");
    Mix_Quit();
    writeLog("SDL_MIXER ĐÃ ĐƯỢC TẮT");
}

void Menu::render()
{
    if (backgroundTexture)
    {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }
    if (effectButton)
    {
        SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 100);
        SDL_RenderDrawRect(renderer, effectButton);
    }
}

MenuState Menu::handleEvents(SDL_Event &event)
{
    if (event.type == SDL_MOUSEMOTION)
    {
        SDL_Point mousePos = {event.motion.x, event.motion.y};
        effectButton = nullptr;

        if (SDL_PointInRect(&mousePos, &playButton))
            effectButton = &playButton;
        else if (SDL_PointInRect(&mousePos, &continueButton))
            effectButton = &continueButton;
        else if (SDL_PointInRect(&mousePos, &exitButton))
            effectButton = &exitButton;
    }

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
void Menu::loseGameMusic()
{
    if (loseGame)
    {
        Mix_PlayChannel(-1, loseGame, 0); // lặp 1 lần
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