#include "Map.h"
using namespace std;
Map::Map(SDL_Renderer *renderer, int tileSize) : renderer(renderer), tileSize(tileSize), Gameover(false), ShowMap(true)
{
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        cerr << "Lỗi khi khởi tạo SDL_Image: " << IMG_GetError() << endl;
        writeLog("Lỗi khi khởi tạo SDL_Image: " + string(IMG_GetError()));
        return;
    }
    types.resize(3, nullptr);
    const char *fileNames[] = {"image/grass1.png", "image/wall1.png", "image/treasure.png"};
    for (int i = 0; i < 3; i++)
    {
        SDL_Surface *surface = IMG_Load(fileNames[i]);
        if (!surface)
        {
            cerr << "Không thể tải ảnh: " << IMG_GetError() << endl;
            writeLog("Không thể tải ảnh [" + to_string(i) + "]: " + string(IMG_GetError()));
            types[i] = nullptr;
            continue;
        }
        types[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!types[i])
        {
            cerr << "Không thể tạo texture: " << SDL_GetError() << endl;
            writeLog("Không thể tạo texture [" + to_string(i) + "]: " + string(SDL_GetError()));
        }
        else
        {
            writeLog("types[" + to_string(i) + "] created");
        }
    }
}

Map::~Map()
{
    for (int i = 0; i < 3; i++)
    {
        if (types[i])
        {
            SDL_DestroyTexture(types[i]);
            types[i] = nullptr;
            writeLog("Map types[" + to_string(i) + "] deleted");
        }
    }
    IMG_Quit();
}

bool Map::LoadMap(const string &file)
{
    ifstream File(file);
    if (!File.is_open())
    {
        cerr << "Không thể mở file bản đồ: " << file << endl;
        writeLog("Không thể mở file bản đồ: " + string(file));
        return false;
    }

    tileMap.clear();
    string line;
    while (getline(File, line))
    {
        vector<int> row;
        for (char c : line)
        {
            if (c == '0')
                row.push_back(GRASS);
            else if (c == '1')
                row.push_back(WALL);
            else if (c == '2')
                row.push_back(TREASURE);
            else
                continue;
        }
        tileMap.push_back(row);
    }
    File.close();
    return !tileMap.empty();
}
bool Map::isGameover() const
{
    return Gameover;
}

int Map::getTile(int x, int y, int width, int height)
{
    int leftCol = x / tileSize;
    int rightCol = (x + width - 1) / tileSize;
    int topRow = y / tileSize;
    int bottomRow = (y + height - 1) / tileSize;
    for (int row = topRow; row <= bottomRow; row++)
    {
        for (int col = leftCol; col <= rightCol; col++)
        {
            if (tileMap[row][col] == WALL)
                return 1;
            if (tileMap[row][col] == TREASURE)
            {
                Gameover = true;
                return 2;
            }
        }
    }
    return 0;
}

void Map::render()
{
    if (!ShowMap)
        return;
    else
    {
        int rows = tileMap.size();
        int cols = tileMap[0].size();
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int tileType = tileMap[i][j];
                if (tileType < 0 || tileType >= types.size() || !types[tileType])
                    continue;
                SDL_Rect dstRect = {tileSize * j, tileSize * i, tileSize, tileSize};
                SDL_RenderCopy(renderer, types[tileType], NULL, &dstRect);
            }
        }
    }
}
void Map::setGameover(bool value)
{
    Gameover = value;
}
void Map::setShowMap(bool value)
{
    ShowMap = value;
}