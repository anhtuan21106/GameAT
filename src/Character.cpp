#include "Character.h"
using namespace std;
// Hàm khởi tạo
Character::Character(SDL_Renderer *renderer) : renderer(renderer), musicmove(nullptr), x(4 * 22), y(4 * 22), width(44), height(44), currentFrame(0)
{
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        cerr << "Lỗi khi khởi tạo SDL_Image: " << IMG_GetError() << endl;
        writeLog("Lỗi khi khởi tạo SDL_Image: " + string(IMG_GetError()));
        return;
    }
    // Tạo texture cho người chơi
    const char *imagePaths[12] = {"image/character1.png", "image/character2.png", "image/character3.png", "image/character4.png", "image/c11.png",
                                  "image/c12.png", "image/c21.png", "image/c22.png", "image/c31.png", "image/c32.png", "image/c41.png", "image/c42.png"};
    for (int i = 0; i < 12; i++)
    {
        SDL_Surface *surface = IMG_Load(imagePaths[i]);
        if (!surface)
        {
            cerr << "Không thể tải ảnh: " << IMG_GetError() << endl;
            writeLog("Không thể tải ảnh [" + to_string(i) + "]: " + string(IMG_GetError()));
            characterTextures[i] = nullptr;
            continue;
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
    // Tạo am thanh
    musicmove = Mix_LoadWAV("music/move1.wav");
    if (!musicmove)
    {
        cerr << "LỖI TẢI ÂM THANH di chuyển: " << Mix_GetError() << endl;
        writeLog("LỖI TẢI ÂM THANH di chuyển: " + string(Mix_GetError()));
    }
    else
    {
        Mix_VolumeChunk(musicmove, 90);
        writeLog("ÂM THANH di chuyển TẢI THÀNH CÔNG");
    }
}
// hàm hủy
Character::~Character()
{
    for (int i = 0; i < 12; i++)
    {
        if (characterTextures[i])
        {
            SDL_DestroyTexture(characterTextures[i]);
            characterTextures[i] = nullptr;
            writeLog("characterTextures[" + to_string(i) + "] deleted");
        }
    }
}
// hàm render
void Character::render()
{

    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(renderer, characterTextures[currentFrame], NULL, &rect);
}
// hàm di chuyển
void Character::move(int stepX, int stepY, int newFrame, Map &map)
{
    Mix_PlayChannel(-1, musicmove, 0);
    currentFrame = newFrame;
    int tileType = map.getTile(x + stepX, y + stepY, width, height);
    if (tileType == 0)
    {
        x += stepX;
        y += stepY;
    }
    writeLog("x: " + to_string(x) + ", y: " + to_string(y) + ", currentFrame: " + to_string(currentFrame));
}
// hàm set current position
void Character::setCurrentPosition(int newX, int newY, int newFrame, int timeStart, int Time, TimeManager &timeManager)
{
    x = newX;
    y = newY;
    currentFrame = newFrame;
    timeManager.setTimeStart(timeStart);
    timeManager.setTime(Time);
    writeLog("setCurrentPosition x: " + to_string(x) + ", y: " + to_string(y) + ", currentFrame: " + to_string(currentFrame) + ", timeStart: " + to_string(timeStart) + ", time: " + to_string(Time));
}
// hàm reset position
void Character::resetPosition()
{
    x = 4 * 22;
    y = 4 * 22;
    currentFrame = 0;
    writeLog("resetPosition x: " + to_string(x) + ", y: " + to_string(y) + ", currentFrame: " + to_string(currentFrame));
}
// hàm đọc vị trí vào file
vector<int> Character::getPrePosition(const char *filename, TimeManager &timeManager)
{
    vector<int> prePosition = {x, y, currentFrame, timeManager.getTimeStart(), timeManager.getTime()};
    ifstream File(filename);
    if (!File.is_open())
    {
        cerr << "Không thể mở file: " << filename << endl;
        writeLog("Không thể mở file: " + string(filename));
        return prePosition;
    }
    File >> prePosition[0] >> prePosition[1] >> prePosition[2] >> prePosition[3] >> prePosition[4];
    writeLog("getPrePosition x: " + to_string(prePosition[0]) + ", y: " + to_string(prePosition[1]) + ", currentFrame: " + to_string(prePosition[2]) + ", timeStart: " + to_string(prePosition[3]) + ", time: " + to_string(prePosition[4]));
    File.close();
    return prePosition;
}
// hàm lấy vị trí từ file
void Character::setPrePosition(const char *filename, TimeManager &timeManager)
{
    ofstream File(filename, ios::trunc);
    if (!File.is_open())
    {
        cerr << "Không thể mở file: " << filename << endl;
        writeLog("Không thể mở file: " + string(filename));
        return;
    }
    File << x << " " << y << " " << currentFrame << " " << timeManager.getTimeStart() << " " << timeManager.getTime();
    writeLog("setPrePosition x:" + to_string(x) + ", y: " + to_string(y) + ", currentFrame: " + to_string(currentFrame) + ", timeStart: " + to_string(timeManager.getTimeStart()) + ", time: " + to_string(timeManager.getTime()));
    File.close();
}
